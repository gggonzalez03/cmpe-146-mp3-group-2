#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "lpc40xx.h"
#include "sj2_cli.h"

#include "SSD1306_OLED.h"
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

QueueHandle_t mp3_controller__controls_queue;

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
static void mp3_display_task(void *p);

int main(void) {
  q_songname = xQueueCreate(4, sizeof(songname_t));
  q_songdata = xQueueCreate(2, sizeof(file_buffer_t));

  xTaskCreate(mp3_reader_task, "mp3_reader_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &mp3_reader_task_handle);
  xTaskCreate(mp3_player_task, "mp3_player_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              &mp3_player_task_handle);
  xTaskCreate(mp3_oled_screen_task, "mp3_oled_screen_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(mp3_controller_task, "mp3_controller_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  // xTaskCreate(mp3_reader_task, "mp3_reader_task", 4096 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  // xTaskCreate(mp3_player_task, "mp3_player_task", 4096 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(mp3_display_task, "mp3_display_task", 4096 / sizeof(void *), NULL, PRIORITY_LOW, NULL);

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

      if (result != FR_OK) {
        fprintf(stderr, "File does not exist.\n");
        continue;
      }
    }

    f_close(&file);

    start_of_audio = get_mp3_metadata_from_id3v1_tag(filename, &mp3);

    (void)f_open(&file, filename, (FA_READ));
    f_lseek(&file, start_of_audio);

    print_mp3_metadata(&mp3);

    while (!f_eof(&file)) {

      if (mp3_controller__is_break_required()) {
        break;
      }

      f_read(&file, (void *)buffer, sizeof(file_buffer_t), &bytes_read);
      xQueueSend(q_songdata, (void *)buffer, portMAX_DELAY);
      // vTaskDelay(100);
    }
    f_close(&file);

    mp3_controller__reset_flags();
  }
}

static void mp3_player_task(void *p) {
  file_buffer_t buffer;

  vs1053b__mp3_decoder_initialize();

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

  print_song_list();

  while (1) {
    vTaskDelay(portMAX_DELAY);
  }
}

static void mp3_controller_task(void *p) {

  mp3_controller__controls_queue = xQueueCreate(10, sizeof(mp3_controller_s));
  mp3_controller_s mp3_control;

  mp3_controller__initialize();

  while (1) {
    xQueueReceive(mp3_controller__controls_queue, (void *)&mp3_control, portMAX_DELAY);
    mp3_controller__execute_control(&mp3_control);
  }
}
static void mp3_display_task(void *p) {
  SSD1306_OLED_initialize();
  delay__ms(100);
  SSD1306__write(0xAE, NULL);
  SSD1306__write(0xD5, 0x80);
  SSD1306__write(0xA8, 0x3F);
  SSD1306__write(0xD3, 0x20); // set display offset
  SSD1306__write(0x40, NULL);
  SSD1306__write(0x8D, 0x14);
  SSD1306__write(0xA1, NULL);
  SSD1306__write(0xC8, NULL);
  SSD1306__write(0xDA, 0x12);
  SSD1306__write(0x81, 0xCF);
  SSD1306__write(0xD9, 0xF1);
  SSD1306__write(0xDB, 0x40);
  SSD1306__write(0xA4, NULL);
  SSD1306__write(0xA6, NULL);
  SSD1306__write(0xAF, NULL);
  // SSD1306__write(0xA5, NULL);
  SSD1306__write(0x20, 0x00);
  // SSD1306__write(0x21, NULL);

  SSD1306__write(0xA4, NULL);

  uint8_t column_data[] = {0x00, 0x7F};
  uint8_t page_data[] = {0x00, 0x07};

  SSD1306__command_write(0x21, column_data, 2);
  SSD1306__command_write(0x22, page_data, 2);

  for (int i = 0; i < 1024; i++) {
    SSD1306__data_write(0x00);
  }
  // H
  SSD1306__data_write(0xFF);
  SSD1306__data_write(0x08);
  SSD1306__data_write(0x08);
  SSD1306__data_write(0x08);
  SSD1306__data_write(0x08);
  SSD1306__data_write(0xFF);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  //  E
  SSD1306__data_write(0xFF);
  SSD1306__data_write(0x89);
  SSD1306__data_write(0x89);
  SSD1306__data_write(0x89);
  SSD1306__data_write(0x89);
  // SSD1306__data_write(0x08);
  // SSD1306__data_write(0xFF);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  //  L
  SSD1306__data_write(0xFF);
  SSD1306__data_write(0x80);
  SSD1306__data_write(0x80);
  SSD1306__data_write(0x80);
  SSD1306__data_write(0x80);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  //  L
  SSD1306__data_write(0xFF);
  SSD1306__data_write(0x80);
  SSD1306__data_write(0x80);
  SSD1306__data_write(0x80);
  SSD1306__data_write(0x80);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  // O
  SSD1306__data_write(0x7E);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x7E);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  // G
  SSD1306__data_write(0x7E);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x89);
  SSD1306__data_write(0xF8);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  // I
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0xFF);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x81);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  // N
  SSD1306__data_write(0xFF);
  SSD1306__data_write(0x03);
  SSD1306__data_write(0x0C);
  SSD1306__data_write(0x30);
  SSD1306__data_write(0xC0);
  SSD1306__data_write(0xFF);

  // space
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  // G
  SSD1306__data_write(0x7E);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x81);
  SSD1306__data_write(0x89);
  SSD1306__data_write(0xF8);

  // SSD1306__data_write(0xFE);
  // SSD1306__data_write(0x01);
  // SSD1306__data_write(0x01);
  // SSD1306__data_write(0x01);
  // SSD1306__data_write(0x01);

  /*
  for (int i = 0; i < 100; i++) {
    SSD1306__data_write(0xFF);
  }
*/

  // Scrolling command //
  SSD1306__horizontalscroll_on();

  // scrolling end //

  while (1) {
    ;
  }
}

/**********************************************************
 *              Helper Functions Definitions
 **********************************************************/