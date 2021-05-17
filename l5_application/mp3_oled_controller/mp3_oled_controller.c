#include "mp3_oled_controller.h"
#include "mp3_oled_controller_icons.h"

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t mp3_oled_controller__screen_update_queue;

/************************************************************************************
 *
 *                                PRIVATE DECLARATIONS
 *
 ***********************************************************************************/

const uint8_t mp3_oled_controller__max_lines_on_screen = 8;
const uint8_t mp3_oled_controller__oled_start_column = 0x00;
const uint8_t mp3_oled_controller__oled_end_column = 0x7F;
const uint8_t mp3_oled_controller__oled_start_column_margin = 0x0D;
const uint8_t mp3_oled_controller__oled_end_column_margin = 0x72;
const uint8_t mp3_oled_controller__oled_center_horizontal =
    (mp3_oled_controller__oled_end_column - mp3_oled_controller__oled_start_column) / 2;

static mp3_oled_controller_s mp3_oled_screen = {.current_top_song_index = 0,
                                                .current_bottom_song_index = 7,
                                                .highlighted_song_index = 0,
                                                .mp3_playing_song = NULL,
                                                .is_song_paused = true,
                                                .volume_percentage = 70};

const uint32_t mp3_oled_controller__songname_max_length = 12;

static const mp3_oled_controller__screen_update_e song_list_screen = MP3_OLED_CONTROLLER_SHOW_SONG_LIST_SCREEN;
static const mp3_oled_controller__screen_update_e player_screen = MP3_OLED_CONTROLLER_SHOW_PLAYER_SCREEN;

/************************************************************************************
 *
 *                                  PRIVATE FUNCTIONS
 *
 ***********************************************************************************/

static void mp3_oled_controller__player_print_song_metadata(void) {

  uint8_t start_row = 0x00;
  uint8_t end_row = 0x00;

  if (mp3_oled_screen.mp3_playing_song != NULL) {
    SSD1306__page_specify(start_row, end_row);
    SSD1306__column_specify(mp3_oled_controller__oled_start_column_margin, mp3_oled_controller__oled_end_column_margin);
    SSD1306_ascii_display_string_with_max_length(mp3_oled_screen.mp3_playing_song->song_title,
                                                 &mp3_oled_controller__songname_max_length, false);

    SSD1306__page_specify(++start_row, ++end_row);
    SSD1306__column_specify(mp3_oled_controller__oled_start_column_margin, mp3_oled_controller__oled_end_column_margin);
    SSD1306_ascii_display_string_with_max_length(mp3_oled_screen.mp3_playing_song->artist,
                                                 &mp3_oled_controller__songname_max_length, false);

    SSD1306__page_specify(++start_row, ++end_row);
    SSD1306__column_specify(mp3_oled_controller__oled_start_column_margin, mp3_oled_controller__oled_end_column_margin);
    SSD1306_ascii_display_string_with_max_length(mp3_oled_screen.mp3_playing_song->album,
                                                 &mp3_oled_controller__songname_max_length, false);

    // SSD1306__page_specify(++start_row, ++end_row);
    // SSD1306__column_specify(mp3_oled_controller__oled_start_column_margin,
    // mp3_oled_controller__oled_end_column_margin);
    // SSD1306_ascii_display_string_with_max_length(mp3_oled_screen.mp3_playing_song->year,
    //                                             &mp3_oled_controller__songname_max_length, false);
  }
}

static void mp3_oled_controller__song_list_show_prv(void) {
  uint8_t start_row = 0x00;
  uint8_t end_row = 0x00;
  uint8_t start_column = mp3_oled_controller__oled_start_column_margin;
  uint8_t end_column = mp3_oled_controller__oled_end_column_margin;

  SSD1306__clear_screen();

  for (size_t index = 0; index < mp3_oled_controller__max_lines_on_screen; index++) {
    SSD1306__page_specify(start_row + index, end_row + index);
    SSD1306__column_specify(start_column, end_column);

    if (mp3_oled_screen.highlighted_song_index == mp3_oled_screen.current_top_song_index + index) {
      SSD1306_ascii_display_string_with_max_length(
          mp3_song_list__get_name_for_item(mp3_oled_screen.current_top_song_index + index),
          &mp3_oled_controller__songname_max_length, true);
      continue;
    }

    SSD1306_ascii_display_string_with_max_length(
        mp3_song_list__get_name_for_item(mp3_oled_screen.current_top_song_index + index),
        &mp3_oled_controller__songname_max_length, false);
  }

  mp3_oled_controller_icons__print_left_nav(false);
  mp3_oled_controller_icons__print_right_nav(false);
}

static void mp3_oled_controller__player_show_prv(void) {
  SSD1306__clear_screen();
  mp3_oled_controller__player_print_song_metadata();
  mp3_oled_controller_icons__print_left_nav(true);

  if (mp3_oled_screen.is_song_paused) {
    mp3_oled_controller_icons__print_play_icon();
  } else {
    mp3_oled_controller_icons__print_pause_icon();
  }
  mp3_oled_controller_icons__print_next_song_icon();
  mp3_oled_controller_icons__print_volume_bar_icon(mp3_oled_screen.volume_percentage);
}

/************************************************************************************
 *
 *                                   PUBLIC FUNCTIONS
 *
 ***********************************************************************************/

/**
 * Initialize MP3 OLED controller
 **/
void mp3_oled_controller__initialize(void) {
  SSD1306__initialize();
  mp3_song_list__populate();
  xQueueSend(mp3_oled_controller__screen_update_queue, (void *)&song_list_screen, 0);
}

/**
 * Show the song list screen
 **/
void mp3_oled_controller__song_list_show(void) {
  xQueueSend(mp3_oled_controller__screen_update_queue, (void *)&song_list_screen, 0);
}

/**
 * Highlight a song in the list
 *
 * If the desired song is out of the screen, automatically scroll up or down.
 *
 * @param song_index is the index of the song to be highlighted
 **/
void mp3_oled_controller__song_list_highlight_song(size_t song_index) {

  if (song_index < mp3_oled_screen.current_top_song_index) {
    mp3_oled_screen.current_top_song_index = song_index;
    mp3_oled_screen.current_bottom_song_index = song_index + mp3_oled_controller__max_lines_on_screen - 1;
  } else if (song_index > mp3_oled_screen.current_bottom_song_index) {
    mp3_oled_screen.current_bottom_song_index = song_index;
    mp3_oled_screen.current_top_song_index = song_index - mp3_oled_controller__max_lines_on_screen + 1;
  }

  mp3_oled_screen.highlighted_song_index = song_index;

  xQueueSend(mp3_oled_controller__screen_update_queue, (void *)&song_list_screen, 0);
}

/**
 * Show the player screen
 **/
void mp3_oled_controller__player_show(void) {
  xQueueSend(mp3_oled_controller__screen_update_queue, (void *)&player_screen, 0);
}

/**
 * Unset the is_song_paused flag
 **/
void mp3_oled_controller__player_set_playing(void) { mp3_oled_screen.is_song_paused = false; }

/**
 * Show the pause icon because there is a song playing currently
 **/
void mp3_oled_controller__player_show_playing(void) {
  mp3_oled_screen.is_song_paused = false;
  xQueueSend(mp3_oled_controller__screen_update_queue, (void *)&player_screen, 0);
}

/**
 * Set the is_song_paused flag
 **/
void mp3_oled_controller__player_set_paused(void) { mp3_oled_screen.is_song_paused = true; }

/**
 * Show the play icon because there's no song currently playing
 **/
void mp3_oled_controller__player_show_paused(void) {
  mp3_oled_screen.is_song_paused = true;
  xQueueSend(mp3_oled_controller__screen_update_queue, (void *)&player_screen, 0);
}

/**
 * Set playing song
 * @param mp3_playing_song is the mp3 song that is playing
 **/
void mp3_oled_controller__player_set_playing_song(const mp3_s *const mp3_playing_song) {
  mp3_oled_screen.mp3_playing_song = mp3_playing_song;
}

/**
 * Set volume
 * @param volume is the new volume level from 0 to 100
 **/
void mp3_oled_controller__player_set_volume(uint8_t volume) { mp3_oled_screen.volume_percentage = volume; }

/**
 * Set and show volume
 * @param volume is the new volume level from 0 to 100
 **/
void mp3_oled_controller__player_show_volume(uint8_t volume) {
  mp3_oled_screen.volume_percentage = volume;
  xQueueSend(mp3_oled_controller__screen_update_queue, (void *)&player_screen, 0);
}

/**
 * Update the screen based on the current data values and flag statuses
 * @param update_id is the id of the type of update to be done
 **/
void mp3_oled_controller__update_screen(mp3_oled_controller__screen_update_e update_id) {

  switch (update_id) {
  case MP3_OLED_CONTROLLER_SHOW_SONG_LIST_SCREEN:
    mp3_oled_controller__song_list_show_prv();
    break;
  case MP3_OLED_CONTROLLER_SHOW_PLAYER_SCREEN:
    mp3_oled_controller__player_show_prv();
    break;

  default:
    break;
  }
}