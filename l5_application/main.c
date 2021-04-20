#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "lpc40xx.h"
#include "sj2_cli.h"

#include "ff.h"
#include "vs1053b_mp3_decoder.h"

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

static void mp3_decoder_test_task(void *p) {
  vs1053b__mp3_decoder_initialize();
  uint16_t status = 0x00;
  bool dreq;
  while (1) {
    status = vs1053b__get_status();
    dreq = vs1053b__mp3_decoder_needs_data();

    uint8_t middle_c = 0b11100111;

    vs1053b__sine_test(middle_c, 1000);
    printf("%d, %d\n", status, dreq);
    vTaskDelay(3000);
  }
}

static void mp3_decoder_decode_header_task(void *p) {
  vs1053b__mp3_song song;

  char filename[] = "life_is_a_dream.mp3";
  file_buffer_t buffer;
  UINT bytes_read;

  FIL file;

  FRESULT result = f_open(&file, filename, (FA_READ));

  if (result == FR_OK) {
    // f_read(&file, (void *)song.header, 10, &bytes_read);
    // f_read(&file, (void *)song.tag, 50, &bytes_read);
  }

  while (1) {
    for (int i = 0; i < 10; i++) {
      printf("0x%x ", song.header[i]);
      // printf("%c ", song.header[i]);
    }
    // printf("%s, %s\n", song.header, song.tag);
    printf("\n");
    for (int i = 0; i < 50; i++) {
      printf("0x%x ", song.tag[i]);
      // printf("%c ", song.tag[i]);
    }
    printf("\n");
    vTaskDelay(1000);
  }
}

static void send_to_mp3_decoder(char byte) { printf("%c", byte); }
static bool mp3_decoder_needs_data() { return true; }

int main(void) {
  q_songname = xQueueCreate(1, sizeof(songname_t));
  q_songdata = xQueueCreate(2, sizeof(file_buffer_t));

  xTaskCreate(mp3_reader_task, "mp3_reader_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(mp3_player_task, "mp3_player_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  // xTaskCreate(mp3_decoder_test_task, "mp3_decoder_test_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  // xTaskCreate(mp3_decoder_decode_header_task, "mp3_decoder_decode_header_task", 4096 / sizeof(void *), NULL,
  // PRIORITY_MEDIUM, NULL);

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
    vs1053b__dcs();
    for (uint16_t i = 0; i < sizeof(file_buffer_t); i++) {
      while (!vs1053b__mp3_decoder_needs_data()) {
        vTaskDelay(1);
      }
      vs1053b__mp3_decoder_play_byte(buffer[i]);
    }
    vs1053b__dds();
  }
}
/**********************************************************
 *              Helper Functions Definitions
 **********************************************************/