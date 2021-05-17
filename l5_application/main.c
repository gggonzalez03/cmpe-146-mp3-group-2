#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "lpc40xx.h"
#include "sj2_cli.h"

#include "ff.h"
#include "mp3_controller.h"
#include "mp3_controller_acc.h"
#include "mp3_metadata_decoder.h"
#include "mp3_oled_controller.h"
#include "vs1053b_mp3_decoder.h"

/**********************************************************
 *                     Declarations
 **********************************************************/
typedef char songname_t[32];
typedef char file_buffer_t[512];

QueueHandle_t q_songname;
QueueHandle_t q_songname_previous;
static QueueHandle_t q_songdata;

QueueHandle_t mp3_controller__control_inputs_queue;
QueueHandle_t mp3_oled_controller__screen_update_queue;

TaskHandle_t mp3_reader_task_handle;
TaskHandle_t mp3_player_task_handle;

/**********************************************************
 *                    Helper Functions
 **********************************************************/
static void song_begin(mp3_s *mp3) {
  mp3_controller__set_is_song_playing_flag();

  mp3_oled_controller__player_set_playing_song(mp3);

  if (mp3_controller__is_on_player_screen()) {
    mp3_oled_controller__player_show_playing();
  } else {
    mp3_oled_controller__player_set_playing();
  }
}

static void song_end(void) {
  mp3_controller__reset_flags();

  if (mp3_controller__is_on_player_screen()) {
    mp3_oled_controller__player_show_paused();
  } else {
    mp3_oled_controller__player_set_paused();
  }
}

/**********************************************************
 * 								    Tasks Prototypes
 **********************************************************/
static void mp3_reader_task(void *p);
static void mp3_player_task(void *p);
static void mp3_oled_screen_task(void *p);
static void mp3_controller_task(void *p);

/**********************************************************
 * 								        Main
 **********************************************************/
int main(void) {
  q_songname = xQueueCreate(10, sizeof(songname_t));
  q_songname_previous = xQueueCreate(10, sizeof(songname_t));
  q_songdata = xQueueCreate(2, sizeof(file_buffer_t));

  xTaskCreate(mp3_reader_task, "mp3_reader_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &mp3_reader_task_handle);
  xTaskCreate(mp3_player_task, "mp3_player_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &mp3_player_task_handle);
  xTaskCreate(mp3_oled_screen_task, "mp3_oled_screen_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(mp3_controller_task, "mp3_controller_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);

  mp3_controller_acc__create_all_tasks();

  sj2_cli__init();
  vTaskStartScheduler();
}

/**********************************************************
 * 								    Tasks Definitions
 **********************************************************/
static void mp3_reader_task(void *p) {

  songname_t filename;
  file_buffer_t buffer;
  UINT bytes_read;

  FIL file;
  FRESULT result;

  mp3_s mp3;
  uint32_t start_of_audio;

  while (1) {

    if (xQueueReceive(q_songname, (void *)filename, portMAX_DELAY)) {
      result = f_open(&file, filename, (FA_READ));
      f_close(&file);

      if (result != FR_OK) {
        fprintf(stderr, "File does not exist.\n");
        continue;
      }
    }

    start_of_audio = get_mp3_metadata_from_id3v1_tag(filename, &mp3);

    (void)f_open(&file, filename, (FA_READ));
    f_lseek(&file, start_of_audio);

    song_begin(&mp3);

    while (!f_eof(&file)) {

      if (mp3_controller__is_break_required()) {
        xQueueReset(q_songdata);
        break;
      }

      f_read(&file, (void *)buffer, sizeof(file_buffer_t), &bytes_read);
      xQueueSend(q_songdata, (void *)buffer, portMAX_DELAY);
    }
    f_close(&file);

    if (!mp3_controller__is_break_required()) {
      xQueueSendToFront(q_songname_previous, (void *)filename, 0);
    }

    song_end();
  }
}

static void mp3_player_task(void *p) {
  file_buffer_t buffer;
  uint8_t volume_70_percent = 70;

  vs1053b__mp3_decoder_initialize();
  vs1053b__set_volume(volume_70_percent, volume_70_percent);

  while (1) {
    xQueueReceive(q_songdata, (void *)buffer, portMAX_DELAY);
    vs1053b__mp3_decoder_start();
    for (uint16_t i = 0; i < sizeof(file_buffer_t); i++) {
      while (!vs1053b__mp3_decoder_needs_data()) {
        vTaskDelay(1);
      }
      vs1053b__mp3_decoder_play_byte(buffer[i]);
    }
    vs1053b__mp3_decoder_end();
  }
}

static void mp3_oled_screen_task(void *p) {

  mp3_oled_controller__screen_update_queue = xQueueCreate(10, sizeof(mp3_oled_controller__screen_update_e));
  mp3_oled_controller__screen_update_e update_id;

  mp3_oled_controller__initialize();

  while (1) {
    xQueueReceive(mp3_oled_controller__screen_update_queue, (void *)&update_id, portMAX_DELAY);
    mp3_oled_controller__update_screen(update_id);
  }
}

static void mp3_controller_task(void *p) {

  mp3_controller__control_inputs_queue = xQueueCreate(10, sizeof(mp3_controller__control_input_source_e));
  mp3_controller__control_input_source_e mp3_control_input;
  mp3_controller_s mp3_control;

  mp3_controller__initialize();

  while (1) {
    xQueueReceive(mp3_controller__control_inputs_queue, (void *)&mp3_control_input, portMAX_DELAY);
    mp3_control = mp3_controller__decode_control_from_input(mp3_control_input);
    mp3_controller__execute_control(&mp3_control);
  }
}
/**********************************************************
 *              Helper Functions Definitions
 **********************************************************/
