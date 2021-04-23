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

/**********************************************************
 *                     Declarations
 **********************************************************/
typedef char songname_t[32];
typedef char file_buffer_t[512];

QueueHandle_t q_songname;
static QueueHandle_t q_songdata;

/**********************************************************
 *                    Helper Functions
 **********************************************************/

/**********************************************************
 * 								    Tasks Prototypes
 **********************************************************/
static void mp3_reader_task(void *p);
static void mp3_player_task(void *p);
static void mp3_display_task(void *p);

int main(void) {
  q_songname = xQueueCreate(1, sizeof(songname_t));
  q_songdata = xQueueCreate(2, sizeof(file_buffer_t));

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

  char filename;
  file_buffer_t buffer;
  UINT bytes_read;

  FIL file;
  FRESULT result;

  while (1) {

    if (xQueueReceive(q_songname, (void *)&filename, portMAX_DELAY)) {
      result = f_open(&file, &filename, (FA_READ));

      if (result != FR_OK) {
        fprintf(stderr, "File does not exist.\n");
        continue;
      }
    }

    while (!f_eof(&file)) {
      f_read(&file, (void *)buffer, sizeof(file_buffer_t), &bytes_read);
      xQueueSend(q_songdata, (void *)buffer, portMAX_DELAY);
      // vTaskDelay(100);
    }
    f_close(&file);
    printf("Song done\n");
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

static void mp3_display_task(void *p) {
  SSD1306_OLED_initialize();
  delay__ms(100);
  SSD1306__write(0xAE, NULL);
  SSD1306__write(0xD5, 0x80);
  SSD1306__write(0xA8, 0x3F);
  SSD1306__write(0xD3, 0x00);
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
  SSD1306__write(0xA5, NULL);
  SSD1306__write(0x20, 0x00);
  SSD1306__write(0x21, NULL);

  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);
  SSD1306__data_write(0x00);

  while (1) {
    ;
  }
}

/**********************************************************
 *              Helper Functions Definitions
 **********************************************************/