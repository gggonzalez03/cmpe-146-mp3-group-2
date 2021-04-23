#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "lpc40xx.h"
#include "sj2_cli.h"

#include "ff.h"
#include "mp3_metadata_decoder.h"
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

void print_mp3_metadata(mp3_s *mp3) {
  printf("File Identifier: %s\n", mp3->tag);
  printf("ID3v2 version: %d, revision: %d\n", mp3->id3_version[0], mp3->id3_version[1]);
  printf("ID3v2 flags: %d\n", mp3->id3_flags);
  printf("ID3v2 size (in bytes): %ld\n", mp3->id3_size_in_bytes);
}

/**********************************************************
 * 								    Tasks Prototypes
 **********************************************************/
static void mp3_reader_task(void *p);
static void mp3_player_task(void *p);

int main(void) {
  q_songname = xQueueCreate(1, sizeof(songname_t));
  q_songdata = xQueueCreate(2, sizeof(file_buffer_t));

  xTaskCreate(mp3_reader_task, "mp3_reader_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(mp3_player_task, "mp3_player_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);

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

  mp3_s mp3;

  while (1) {

    if (xQueueReceive(q_songname, (void *)&filename, portMAX_DELAY)) {
      result = f_open(&file, &filename, (FA_READ));

      if (result != FR_OK) {
        fprintf(stderr, "File does not exist.\n");
        continue;
      }
    }

    // This will also update the file pointer to point to the end
    // of the mp3 header (start of the mp3 audio data)
    (void)get_mp3_metadata_from_id3_header(&file, &mp3);
    print_mp3_metadata(&mp3);

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
/**********************************************************
 *              Helper Functions Definitions
 **********************************************************/