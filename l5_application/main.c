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

static void send_to_mp3_decoder(char byte) { printf("%c", byte); }
static bool mp3_decoder_needs_data() { return true; }

int main(void) {
  q_songname = xQueueCreate(1, sizeof(songname_t));
  q_songdata = xQueueCreate(2, sizeof(file_buffer_t));

  // xTaskCreate(mp3_reader_task, "mp3_reader_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  // xTaskCreate(mp3_player_task, "mp3_player_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(mp3_decoder_test_task, "mp3_decoder_test_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);

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

  while (1) {

    if (xQueueReceive(q_songname, (void *)&filename, portMAX_DELAY)) {
      (void)f_open(&file, &filename, (FA_READ));
    }

    while (!f_eof(&file)) {
      f_read(&file, (void *)buffer, sizeof(file_buffer_t), &bytes_read);
      xQueueSend(q_songdata, (void *)buffer, portMAX_DELAY);
      vTaskDelay(1000);
    }
    f_close(&file);
  }
}

static void mp3_player_task(void *p) {
  file_buffer_t buffer;

  while (1) {
    xQueueReceive(q_songdata, (void *)buffer, portMAX_DELAY);
    for (uint16_t i = 0; i < sizeof(file_buffer_t); i++) {
      while (!vs1053b__mp3_decoder_needs_data()) {
        vTaskDelay(1);
      }
      // send_to_mp3_decoder(buffer[i]);
    }
  }
}
/**********************************************************
 *              Helper Functions Definitions
 **********************************************************/