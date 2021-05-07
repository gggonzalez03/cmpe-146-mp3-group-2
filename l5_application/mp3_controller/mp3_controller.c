#include <stddef.h>
#include <stdio.h>

#include "mp3_controller.h"
#include "mp3_oled_controller.h"
#include "mp3_song_list.h"

#include "gpio.h"
#include "gpio_isr.h"
#include "lpc_peripherals.h"
#include "sys_time.h"
#include "vs1053b_mp3_decoder.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

extern QueueHandle_t q_songname;
extern QueueHandle_t mp3_controller__control_inputs_queue;

extern TaskHandle_t mp3_reader_task_handle;
extern TaskHandle_t mp3_player_task_handle;

/************************************************************************************
 *
 *                                PRIVATE DECLARATIONS
 *
 ***********************************************************************************/
static const gpio_s mp3_controller_show_songs_button = {GPIO__PORT_0, 29};
static const gpio_s mp3_controller_show_player_button = {GPIO__PORT_0, 30};
static const gpio_s mp3_controller_rotary_out_clk = {GPIO__PORT_2, 5};
static const gpio_s mp3_controller_rotary_out_dt = {GPIO__PORT_2, 2};
static const gpio_s mp3_controller_rotary_sw = {GPIO__PORT_2, 0};
static const gpio_s mp3_controller_accel_interrupt_1 = {GPIO__PORT_0, 26};
static const gpio_s mp3_controller_accel_interrupt_2 = {GPIO__PORT_0, 25};

static const mp3_controller_s show_songs = {MP3_CONTROLLER__SHOW_SONGS, 0};
static const mp3_controller_s show_player = {MP3_CONTROLLER__SHOW_PLAYER, 0};
static const mp3_controller_s play_enqueued_song = {MP3_CONTROLLER__PLAY_ENQUEUED_SONG, 0};
static const mp3_controller_s scroll_up = {MP3_CONTROLLER__SCROLL_UP, 0};
static const mp3_controller_s scroll_down = {MP3_CONTROLLER__SCROLL_DOWN, 0};
static const mp3_controller_s volume_up = {MP3_CONTROLLER__VOLUME_UP, 0};
static const mp3_controller_s volume_down = {MP3_CONTROLLER__VOLUME_DOWN, 0};

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

static void mp3_controller__pause_song_accel_callback(void) {}

static void mp3_controller__resume_song_accel_callback(void) {}

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
      mp3_controller__rotary_sw_callback, mp3_controller__pause_song_accel_callback,
      mp3_controller__resume_song_accel_callback};

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
    }
    break;
  case MP3_CONTROLLER__SCROLL_DOWN:
    scroll_index = (++scroll_index) % number_of_songs;
    break;
  case MP3_CONTROLLER__VOLUME_UP:
    if (volume_percentage < 100) {
      ++volume_percentage;
      vs1053b__set_volume(volume_percentage, volume_percentage);
    }
    break;
  case MP3_CONTROLLER__VOLUME_DOWN:
    if (volume_percentage > 0) {
      --volume_percentage;
      vs1053b__set_volume(volume_percentage, volume_percentage);
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
}

void mp3_controller__enqueue_song(size_t item_number) {
  const char *songname = mp3_song_list__get_name_for_item(item_number);
  xQueueSend(q_songname, (void *)songname, 0);
}

void mp3_controller__pause_song(void) {
  vTaskSuspend(mp3_reader_task_handle);
  is_song_paused = true;
}

void mp3_controller__resume_song(void) {
  vTaskResume(mp3_reader_task_handle);
  is_song_paused = false;
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
}

void mp3_controller__go_to_player_screen(void) { is_on_player_screen = true; }

void mp3_controller__go_to_song_list_screen(void) { is_on_player_screen = false; }

void mp3_controller__reset_flags(void) {
  // We assume that a song is stopped for another song to play
  // either by playing a song directly from the song list or
  // by skipping to to play an enqueued song. What this means is that
  // there is still a song playing but a break was required to stop
  // the previous song.
  is_song_playing = is_break_required;
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
  mp3_controller_s control = show_player; // default is to show the player screen

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
      if (xQueuePeekFromISR(q_songname, &songname) == pdTRUE) {
        control = play_enqueued_song;
      }
    } else {
      control = show_player;
    }
    break;
  case MP3_CONTROLLER__ACCELEROMETER_INT1:
    break;
  case MP3_CONTROLLER__ACCELEROMETER_INT2:
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
    mp3_oled_controller__song_list_show();
    break;
  case MP3_CONTROLLER__SHOW_PLAYER:
    mp3_controller__go_to_player_screen();
    mp3_oled_controller__player_show();
    break;
  case MP3_CONTROLLER__SCROLL_UP:
    mp3_controller__scroll(control);
    mp3_oled_controller__song_list_highlight_song(scroll_index);
    break;
  case MP3_CONTROLLER__SCROLL_DOWN:
    mp3_controller__scroll(control);
    mp3_oled_controller__song_list_highlight_song(scroll_index);
    break;
  case MP3_CONTROLLER__VOLUME_UP:
    mp3_controller__scroll(control);
    mp3_oled_controller__player_set_volume(volume_percentage);
    break;
  case MP3_CONTROLLER__VOLUME_DOWN:
    mp3_controller__scroll(control);
    mp3_oled_controller__player_set_volume(volume_percentage);
    break;
  case MP3_CONTROLLER__PLAY_SONG:
    mp3_controller__play_song(control->argument);
    mp3_controller__go_to_player_screen();
    mp3_oled_controller__player_show_playing();
    break;
  case MP3_CONTROLLER__PLAY_ENQUEUED_SONG:
    mp3_controller__play_enqueued_song();
    mp3_oled_controller__player_show_playing();
    break;
  case MP3_CONTROLLER__ENQUEUE_SONG:
    mp3_controller__enqueue_song(control->argument);
    break;
  case MP3_CONTROLLER__PAUSE_SONG:
    mp3_controller__pause_song();
    mp3_oled_controller__player_show_paused();
    break;
  case MP3_CONTROLLER__RESUME_SONG:
    mp3_controller__resume_song();
    mp3_oled_controller__player_show_playing();
    break;
  case MP3_CONTROLLER__STOP_SONG:
    mp3_controller__stop_song();
    break;

  default:
    break;
  }

  return true;
}