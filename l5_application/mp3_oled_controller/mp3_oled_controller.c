#include "mp3_oled_controller.h"

/************************************************************************************
 *
 *                                PRIVATE DECLARATIONS
 *
 ***********************************************************************************/
static mp3_oled_controller_s mp3_oled_screen = {.current_top_song_index = 0,
                                                .current_bottom_song_index = 7,
                                                .highlighted_song_index = 0,
                                                .playing_song_index = -1,
                                                .max_lines_on_screen = 8,
                                                .is_song_playing = false,
                                                .is_song_paused = false,
                                                .is_on_player_screen = false};

static const uint8_t oled_start_column = 0x00;
static const uint8_t oled_end_column = 0x7F;

static const uint8_t oled_start_column_margin = 0x0A;
static const uint8_t oled_end_column_margin = 0x75;

/************************************************************************************
 *
 *                                  PRIVATE FUNCTIONS
 *
 ***********************************************************************************/

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
  mp3_oled_controller__song_list_show();
}

/**
 * Show the song list screen
 **/
void mp3_oled_controller__song_list_show(void) {
  uint8_t start_row = 0x00;
  uint8_t end_row = 0x00;
  uint8_t start_column = oled_start_column_margin;
  uint8_t end_column = oled_end_column_margin;
  uint32_t max_length = 10;

  SSD1306__clear_screen();

  for (size_t index = 0; index < mp3_oled_screen.max_lines_on_screen; index++) {
    SSD1306__page_specify(start_row + index, end_row + index);
    SSD1306__column_specify(start_column, end_column);

    if (mp3_oled_screen.highlighted_song_index == mp3_oled_screen.current_top_song_index + index) {
      /**
       * TODO: Print highlighted songname
       **/
      SSD1306_ascii_display_string_with_max_length(
          mp3_song_list__get_name_for_item(mp3_oled_screen.current_top_song_index + index), &max_length, true);
      continue;
    }

    SSD1306_ascii_display_string_with_max_length(
        mp3_song_list__get_name_for_item(mp3_oled_screen.current_top_song_index + index), &max_length, false);
  }
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
    mp3_oled_screen.current_bottom_song_index = song_index + mp3_oled_screen.max_lines_on_screen - 1;
  } else if (song_index > mp3_oled_screen.current_bottom_song_index) {
    mp3_oled_screen.current_bottom_song_index = song_index;
    mp3_oled_screen.current_top_song_index = song_index - mp3_oled_screen.max_lines_on_screen + 1;
  }

  mp3_oled_screen.highlighted_song_index = song_index;

  mp3_oled_controller__song_list_show();
}

/**
 * Highlight next song.
 *
 * If the next song is out of the screen, automatically scroll up.
 **/
void mp3_oled_controller__song_list_highlight_next(void);

/**
 * Highlight previous song
 *
 * If the previous song is out of the screen, automatically scroll down.
 **/
void mp3_oled_controller__song_list_highlight_previous(void);

/**
 * Scroll up the song list
 **/
void mp3_oled_controller__song_list_scroll_up(void);

/**
 * Scroll down the song list
 **/
void mp3_oled_controller__song_list_scroll_down(void);

/**
 * Show the player screen
 **/
void mp3_oled_controller__player_show(void);

/**
 * Set playing song
 * @param playing_song_index is the index of the playing song
 **/
void mp3_oled_controller__player_set_playing_song(size_t playing_song_index);

/**
 * Set volume
 * @param volume is the new volume level from 0 to 100
 **/
void mp3_oled_controller__player_set_volume(uint8_t volume);