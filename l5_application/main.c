#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "lpc40xx.h"
#include "sj2_cli.h"

#include "SSD1306_OLED.h"
#include "SSD1306_OLED_ascii.h"

#include "ff.h"
#include "mp3_controller.h"
#include "mp3_metadata_decoder.h"
#include "mp3_song_list.h"
#include "vs1053b_mp3_decoder.h"

/**********************************************************
 *                     Declarations
 **********************************************************/
typedef char songname_t[32];
typedef char file_buffer_t[512];

QueueHandle_t q_songname;
static QueueHandle_t q_songdata;

QueueHandle_t mp3_controller__control_inputs_queue;

TaskHandle_t mp3_reader_task_handle;
TaskHandle_t mp3_player_task_handle;

/**********************************************************
 *                    Helper Functions
 **********************************************************/

static void print_mp3_metadata(mp3_s *mp3) {
  // printf("File Identifier: %s\n", mp3->tag);
  // printf("ID3v2 version: %d, revision: %d\n", mp3->id3_version[0], mp3->id3_version[1]);
  // printf("ID3v2 flags: %d\n", mp3->id3_flags);
  // printf("ID3v2 size (in bytes): %lu\n", mp3->id3_size_in_bytes);
  printf("------------------------------------------------------------------------------\n");
  printf("Song name: %s\n", mp3->song_title);
  printf("Artist: %s\n", mp3->artist);
  printf("Album: %s\n", mp3->album);
  printf("Year: %s\n", mp3->year);
  printf("------------------------------------------------------------------------------\n");
}

static void print_song_list(void) {
  mp3_song_list__populate();
  for (size_t song_number = 0; song_number < mp3_song_list__get_item_count(); song_number++) {
    printf("Song %2d: %s\n", (1 + song_number), mp3_song_list__get_name_for_item(song_number));
  }
}

/**********************************************************
 * 								    Tasks Prototypes
 **********************************************************/
static void mp3_reader_task(void *p);
static void mp3_player_task(void *p);
static void mp3_oled_screen_task(void *p);
static void mp3_controller_task(void *p);

int main(void) {
  q_songname = xQueueCreate(4, sizeof(songname_t));
  q_songdata = xQueueCreate(2, sizeof(file_buffer_t));

  xTaskCreate(mp3_reader_task, "mp3_reader_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &mp3_reader_task_handle);
  xTaskCreate(mp3_player_task, "mp3_player_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &mp3_player_task_handle);
  xTaskCreate(mp3_oled_screen_task, "mp3_oled_screen_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(mp3_controller_task, "mp3_controller_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);

  sj2_cli__init();
  vTaskStartScheduler();
}

/**********************************************************
 * 								    Tasks Definitions
 **********************************************************/
static void mp3_reader_task(void *p) {

  char filename[32];
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

    print_mp3_metadata(&mp3);

    // Inform the mp3 controller that the queued song is played automatically
    mp3_controller__set_is_song_playing_flag();

    while (!f_eof(&file)) {

      if (mp3_controller__is_break_required()) {
        xQueueReset(q_songdata);
        break;
      }

      f_read(&file, (void *)buffer, sizeof(file_buffer_t), &bytes_read);
      xQueueSend(q_songdata, (void *)buffer, portMAX_DELAY);
    }
    f_close(&file);

    mp3_controller__reset_flags();
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

  // print_song_list();

  SSD1306__initialize();

  // Before scrolling o
  SSD1306__displaymenu_test1();
  vTaskDelay(3000);
  // After scrolling one row up
  SSD1306__displaymenu_test2();

  while (1) {
    vTaskDelay(portMAX_DELAY);
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
