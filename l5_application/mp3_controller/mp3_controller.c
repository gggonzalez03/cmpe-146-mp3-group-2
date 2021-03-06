#include <stddef.h>

#include "mp3_controller.h"
#include "mp3_oled_controller.h"
#include "mp3_song_list.h"

#include "acceleration.h"
#include "gpio.h"
#include "gpio_isr.h"
#include "lpc_peripherals.h"
#include "mp3_controller_acc.h"
#include "sys_time.h"
#include "vs1053b_mp3_decoder.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

// accelerometer orientation is different from the actual orientation
// of the mp3. This is the mapping
#define MP3__ORIENTATION_UP ACC__ORIENTATION_LEFT
#define MP3__ORIENTATION_DOWN ACC__ORIENTATION_RIGHT
#define MP3__ORIENTATION_RIGHT ACC__ORIENTATION_DOWN
#define MP3__ORIENTATION_LEFT ACC__ORIENTATION_UP
#define MP3__ORIENTATION_FRONT ACC__ORIENTATION_FRONT
#define MP3__ORIENTATION_BACK ACC__ORIENTATION_BACK

extern QueueHandle_t q_songname;
extern QueueHandle_t q_songname_previous;
extern QueueHandle_t mp3_controller__control_inputs_queue;

extern TaskHandle_t mp3_reader_task_handle;
extern TaskHandle_t mp3_player_task_handle;

/************************************************************************************
 *
 *                                PRIVATE DECLARATIONS
 *
 ***********************************************************************************/
typedef char songname_t[32];

static const gpio_s mp3_controller_show_songs_button = {GPIO__PORT_0, 29};
static const gpio_s mp3_controller_show_player_button = {GPIO__PORT_0, 30};
static const gpio_s mp3_controller_rotary_out_clk = {GPIO__PORT_2, 5};
static const gpio_s mp3_controller_rotary_out_dt = {GPIO__PORT_2, 2};
static const gpio_s mp3_controller_rotary_sw = {GPIO__PORT_2, 0};
static const gpio_s mp3_controller_accel_interrupt_1 = {GPIO__PORT_0, 26};
static const gpio_s mp3_controller_accel_interrupt_2 = {GPIO__PORT_0, 25};

static const mp3_controller_s show_songs = {MP3_CONTROLLER__SHOW_SONGS, 0};
static const mp3_controller_s show_player = {MP3_CONTROLLER__SHOW_PLAYER, 0};
static const mp3_controller_s do_nothing = {MP3_CONTROLLER__DO_NOTHING, 0};
static const mp3_controller_s play_enqueued_song = {MP3_CONTROLLER__PLAY_ENQUEUED_SONG, 0};
static const mp3_controller_s play_previous_song = {MP3_CONTROLLER__PLAY_PREVIOUS_SONG, 0};
static const mp3_controller_s scroll_up = {MP3_CONTROLLER__SCROLL_UP, 0};
static const mp3_controller_s scroll_down = {MP3_CONTROLLER__SCROLL_DOWN, 0};
static const mp3_controller_s volume_up = {MP3_CONTROLLER__VOLUME_UP, 0};
static const mp3_controller_s volume_down = {MP3_CONTROLLER__VOLUME_DOWN, 0};
static const mp3_controller_s set_treble = {MP3_CONTROLLER__SET_TREBLE, 0};
static const mp3_controller_s set_bass = {MP3_CONTROLLER__SET_BASS, 0};
static const mp3_controller_s clear_treble_bass = {MP3_CONTROLLER__CLEAR_TREBLE_BASS, 0};

static const mp3_controller_s play_song = {MP3_CONTROLLER__PLAY_SONG, 0};
static const mp3_controller_s enqueue_song = {MP3_CONTROLLER__ENQUEUE_SONG, 0};
static const mp3_controller_s pause_song = {MP3_CONTROLLER__PAUSE_SONG, 0};
static const mp3_controller_s resume_song = {MP3_CONTROLLER__RESUME_SONG, 0};

static size_t scroll_index = 0;
static uint8_t volume_percentage = 70;

static bool is_song_playing = false;
static bool is_song_paused = false;
static bool is_break_required = false;

static bool is_on_player_screen = false; // the user is either on the player screen or the song list screen

extern songname_t filename;

/************************************************************************************
 *
 *                                  PRIVATE FUNCTIONS
 *
 ***********************************************************************************/

static void mp3_controller__show_songs_button_callback(void) {
  static uint64_t old_timestamp = 0;
  static uint64_t new_timestamp = 0;
  static const mp3_controller__control_input_source_e input = MP3_CONTROLLER__LEFT_BUTTON;

  new_timestamp = sys_time__get_uptime_ms();

  if (new_timestamp - old_timestamp > 200) {
    xQueueSendFromISR(mp3_controller__control_inputs_queue, (void *)&input, NULL);
  }
  old_timestamp = new_timestamp;
}

static void mp3_controller__show_player_button_callback(void) {
  static uint64_t old_timestamp = 0;
  static uint64_t new_timestamp = 0;
  static const mp3_controller__control_input_source_e input = MP3_CONTROLLER__RIGHT_BUTTON;

  new_timestamp = sys_time__get_uptime_ms();

  if (new_timestamp - old_timestamp > 200) {
    xQueueSendFromISR(mp3_controller__control_inputs_queue, (void *)&input, NULL);
  }

  old_timestamp = new_timestamp;
}

static void mp3_controller__rotary_out_clk_falling_callback(void) {
  static uint64_t old_timestamp = 0;
  static uint64_t new_timestamp = 0;
  static const mp3_controller__control_input_source_e input_clockwise = MP3_CONTROLLER__ROTARY_ENCODER_SCROLL_CLOCKW;
  static const mp3_controller__control_input_source_e input_anti_clockwise =
      MP3_CONTROLLER__ROTARY_ENCODER_SCROLL_ANTI_CLOCKW;

  new_timestamp = sys_time__get_uptime_ms();

  /**
   * TODO:
   * The debouncing problem may also be solved in hardware
   **/
  if (new_timestamp - old_timestamp > 40) {
    if (gpio__get(mp3_controller_rotary_out_dt)) {
      xQueueSendFromISR(mp3_controller__control_inputs_queue, (void *)&input_clockwise, NULL);
    } else {
      xQueueSendFromISR(mp3_controller__control_inputs_queue, (void *)&input_anti_clockwise, NULL);
    }
  }

  old_timestamp = new_timestamp;
}

static void mp3_controller__rotary_sw_callback(void) {
  static uint64_t old_timestamp = 0;
  static uint64_t new_timestamp = 0;
  static const mp3_controller__control_input_source_e input = MP3_CONTROLLER__ROTARY_ENCODER_BUTTON;

  new_timestamp = sys_time__get_uptime_ms();

  if (new_timestamp - old_timestamp > 200) {
    xQueueSendFromISR(mp3_controller__control_inputs_queue, (void *)&input, NULL);
  }

  old_timestamp = new_timestamp;
}

static void mp3_controller__accel_interrupt_1_callback(void) {
  static const mp3_controller__control_input_source_e input = MP3_CONTROLLER__ACCELEROMETER_INT1;
  xQueueSendFromISR(mp3_controller__control_inputs_queue, (void *)&input, NULL);
}

static void mp3_controller__accel_interrupt_2_callback(void) {
  static const mp3_controller__control_input_source_e input = MP3_CONTROLLER__ACCELEROMETER_INT2;
  xQueueSendFromISR(mp3_controller__control_inputs_queue, (void *)&input, NULL);
}

static void mp3_controller__initialize_accelerometer() {
  acceleration__init();
  acceleration__enable_orientation_interrupts();
  acceleration__set_orientation_debounce_counter(10);
}

static mp3_controller_s mp3_controller__get_control_from_orientation() {

  mp3_controller_s control = do_nothing;

  songname_t songname;

  if (acceleration__get_interrupt_source() == ACC__SRC_LNDPRT) {
    switch (acceleration__get_orientation()) {
    case MP3__ORIENTATION_UP:
      /* init treble increase */
      control = set_treble;
      break;
    case MP3__ORIENTATION_DOWN:
      /* init bass increase */
      control = set_bass;
      break;
    case MP3__ORIENTATION_RIGHT:
      if (xQueuePeek(q_songname, songname, 0) == pdTRUE) {
        control = play_enqueued_song;
      }
      break;
    case MP3__ORIENTATION_LEFT:
      /* previous song */
      if (xQueuePeek(q_songname_previous, songname, 0) == pdTRUE) {
        control = play_previous_song;
      }
      break;
    case MP3__ORIENTATION_FRONT:
      /* treble and bass back to normal. */
      control = clear_treble_bass;
      break;
    case MP3__ORIENTATION_BACK:
      /* pause song */
      control = pause_song;
      control.argument = scroll_index;
      break;

    default:
      break;
    }
  }

  return control;
}

/************************************************************************************
 *
 *                                   PUBLIC FUNCTIONS
 *
 ***********************************************************************************/
#define RISING_CONTROLS_COUNT 5
#define FALLING_CONTROLS_COUNT 1

void mp3_controller__initialize(void) {
  gpio_s mp3_controls_rising[RISING_CONTROLS_COUNT] = {
      mp3_controller_show_songs_button, mp3_controller_show_player_button, mp3_controller_rotary_sw,
      mp3_controller_accel_interrupt_1, mp3_controller_accel_interrupt_2};
  gpio_s mp3_controls_falling[FALLING_CONTROLS_COUNT] = {mp3_controller_rotary_out_clk};

  function_pointer_t callback_functions_rising[RISING_CONTROLS_COUNT] = {
      mp3_controller__show_songs_button_callback, mp3_controller__show_player_button_callback,
      mp3_controller__rotary_sw_callback, mp3_controller__accel_interrupt_1_callback,
      mp3_controller__accel_interrupt_2_callback};

  function_pointer_t callback_functions_falling[FALLING_CONTROLS_COUNT] = {
      mp3_controller__rotary_out_clk_falling_callback};

  for (uint8_t i = 0; i < RISING_CONTROLS_COUNT; i++) {
    gpio__set_as_input(mp3_controls_rising[i]);
    gpiox__attach_interrupt(mp3_controls_rising[i].port_number, mp3_controls_rising[i].pin_number,
                            GPIO_INTR__RISING_EDGE, callback_functions_rising[i]);
  }

  for (uint8_t i = 0; i < FALLING_CONTROLS_COUNT; i++) {
    gpio__set_as_input(mp3_controls_falling[i]);
    gpiox__attach_interrupt(mp3_controls_falling[i].port_number, mp3_controls_falling[i].pin_number,
                            GPIO_INTR__FALLING_EDGE, callback_functions_falling[i]);
  }

  mp3_controller__initialize_accelerometer();

  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, gpiox__interrupt_dispatcher, NULL);
}

void mp3_controller__scroll(const mp3_controller_s *const control) {

  static size_t number_of_songs = 0;

  if (number_of_songs == 0) {
    number_of_songs = mp3_song_list__get_item_count();
  }

  switch (control->control) {
  case MP3_CONTROLLER__SCROLL_UP:
    if (scroll_index > 0) {
      --scroll_index;
      mp3_oled_controller__song_list_highlight_song(scroll_index);
    }
    break;
  case MP3_CONTROLLER__SCROLL_DOWN:
    scroll_index = (++scroll_index) % number_of_songs;
    mp3_oled_controller__song_list_highlight_song(scroll_index);
    break;
  case MP3_CONTROLLER__VOLUME_UP:
    if (volume_percentage < 100) {
      ++volume_percentage;
      vs1053b__set_volume(volume_percentage, volume_percentage);
      mp3_oled_controller__player_show_volume(volume_percentage);
    }
    break;
  case MP3_CONTROLLER__VOLUME_DOWN:
    if (volume_percentage > 0) {
      --volume_percentage;
      vs1053b__set_volume(volume_percentage, volume_percentage);
      mp3_oled_controller__player_show_volume(volume_percentage);
    }
    break;

  default:
    break;
  }
}

size_t mp3_controller__get_scroll_index(void) { return scroll_index; }

void mp3_controller__play_song(size_t item_number) {
  const char *songname = mp3_song_list__get_name_for_item(item_number);

  xQueueSendToFront(q_songname, (void *)songname, 0);

  if (is_song_playing) {
    mp3_controller__stop_song();
  }
  is_song_playing = true;
  mp3_controller__resume_song();
}

void mp3_controller__enqueue_song(size_t item_number) {
  const char *songname = mp3_song_list__get_name_for_item(item_number);
  xQueueSend(q_songname, (void *)songname, 0);
}

void mp3_controller__pause_song(void) {
  vTaskSuspend(mp3_player_task_handle);
  is_song_paused = true;
  mp3_oled_controller__player_show_paused();
}

void mp3_controller__resume_song(void) {
  vTaskResume(mp3_player_task_handle);
  is_song_paused = false;
  mp3_oled_controller__player_show_playing();
}

void mp3_controller__stop_song(void) {
  is_break_required = true;
  is_song_playing = false;
  is_song_paused = false;
}

void mp3_controller__play_enqueued_song(void) {
  if (is_song_playing) {
    mp3_controller__stop_song();
  }
  is_song_playing = true;
  mp3_controller__resume_song();
}

void mp3_controller__play_previous_song(void) {
  songname_t songname;

  if (xQueueReceive(q_songname_previous, (void *)songname, 0)) {
    xQueueSendToFront(q_songname, (void *)songname, 0);

    if (is_song_playing) {
      mp3_controller__stop_song();
    }
    is_song_playing = true;
    mp3_controller__resume_song();

    mp3_controller__go_to_player_screen();
  }
}

void mp3_controller__go_to_player_screen(void) {
  is_on_player_screen = true;
  mp3_oled_controller__player_show();
}

void mp3_controller__go_to_song_list_screen(void) {
  is_on_player_screen = false;
  mp3_oled_controller__song_list_show();
}

void mp3_controller__reset_flags(void) {
  is_song_playing = false;
  is_break_required = false;
  is_song_paused = false;
}

void mp3_controller__set_is_song_playing_flag(void) { is_song_playing = true; }

bool mp3_controller__is_song_playing(void) { return is_song_playing; }

bool mp3_controller__is_song_paused(void) { return is_song_paused; }

bool mp3_controller__is_song_stopped(void) { return !is_song_playing; }

bool mp3_controller__is_break_required(void) { return is_break_required; }

bool mp3_controller__is_on_player_screen(void) { return is_on_player_screen; }

mp3_controller_s mp3_controller__decode_control_from_input(mp3_controller__control_input_source_e control_input) {

  static char songname[32];
  mp3_controller_s control = do_nothing;

  switch (control_input) {
  case MP3_CONTROLLER__ROTARY_ENCODER_SCROLL_CLOCKW:
    if (is_on_player_screen) {
      control = volume_up;
    } else {
      control = scroll_down;
    }
    break;
  case MP3_CONTROLLER__ROTARY_ENCODER_SCROLL_ANTI_CLOCKW:
    if (is_on_player_screen) {
      control = volume_down;
    } else {
      control = scroll_up;
    }
    break;
  case MP3_CONTROLLER__ROTARY_ENCODER_BUTTON:
    if (is_on_player_screen && is_song_playing && !is_song_paused) {
      control = pause_song;
      control.argument = scroll_index;
    } else if (is_on_player_screen && is_song_playing && is_song_paused) {
      control = resume_song;
      control.argument = scroll_index;
    } else {
      control = play_song;
      control.argument = scroll_index;
    }
    break;
  case MP3_CONTROLLER__LEFT_BUTTON:
    if (!is_on_player_screen) {
      control = enqueue_song;
      control.argument = scroll_index;
    } else {
      control = show_songs;
    }
    break;
  case MP3_CONTROLLER__RIGHT_BUTTON:
    if (is_on_player_screen) {
      if (xQueuePeek(q_songname, songname, 0) == pdTRUE) {
        control = play_enqueued_song;
      }
    } else {
      control = show_player;
    }
    break;
  case MP3_CONTROLLER__ACCELEROMETER_INT1:
    break;
  case MP3_CONTROLLER__ACCELEROMETER_INT2:
    control = mp3_controller__get_control_from_orientation();
    break;

  default:
    break;
  }

  return control;
}

bool mp3_controller__execute_control(const mp3_controller_s *const control) {
  switch (control->control) {
  case MP3_CONTROLLER__SHOW_SONGS:
    mp3_controller__go_to_song_list_screen();
    break;
  case MP3_CONTROLLER__SHOW_PLAYER:
    mp3_controller__go_to_player_screen();
    break;
  case MP3_CONTROLLER__SCROLL_UP:
    mp3_controller__scroll(control);
    break;
  case MP3_CONTROLLER__SCROLL_DOWN:
    mp3_controller__scroll(control);
    break;
  case MP3_CONTROLLER__VOLUME_UP:
    mp3_controller__scroll(control);
    break;
  case MP3_CONTROLLER__VOLUME_DOWN:
    mp3_controller__scroll(control);
    break;
  case MP3_CONTROLLER__PLAY_SONG:
    mp3_controller__play_song(control->argument);
    mp3_controller__go_to_player_screen();
    xQueueSendToFront(q_songname_previous, (void *)filename, 0);
    break;
  case MP3_CONTROLLER__PLAY_ENQUEUED_SONG:
    mp3_controller__play_enqueued_song();
    mp3_controller__go_to_player_screen();
    xQueueSendToFront(q_songname_previous, (void *)filename, 0);
    break;
  case MP3_CONTROLLER__ENQUEUE_SONG:
    mp3_controller__enqueue_song(control->argument);
    break;
  case MP3_CONTROLLER__PLAY_PREVIOUS_SONG:
    mp3_controller__play_previous_song();
    break;
  case MP3_CONTROLLER__PAUSE_SONG:
    mp3_controller__pause_song();
    break;
  case MP3_CONTROLLER__RESUME_SONG:
    mp3_controller__resume_song();
    break;
  case MP3_CONTROLLER__STOP_SONG:
    mp3_controller__stop_song();
    break;
  case MP3_CONTROLLER__SET_TREBLE:
    mp3_controller_acc__resume_treble_task();
    mp3_controller__go_to_player_screen();
    break;
  case MP3_CONTROLLER__SET_BASS:
    mp3_controller_acc__resume_bass_task();
    mp3_controller__go_to_player_screen();
    break;
  case MP3_CONTROLLER__CLEAR_TREBLE_BASS:
    mp3_controller_acc__suspend_treble_task();
    mp3_controller_acc__suspend_bass_task();
    break;

  default:
    break;
  }

  return true;
}