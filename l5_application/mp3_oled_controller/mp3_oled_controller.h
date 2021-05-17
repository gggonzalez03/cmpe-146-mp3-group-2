#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "SSD1306_OLED.h"
#include "SSD1306_OLED_ascii.h"

#include "mp3_metadata_decoder.h"
#include "mp3_song_list.h"

typedef enum {
  MP3_OLED_CONTROLLER_SHOW_SONG_LIST_SCREEN = 0,
  MP3_OLED_CONTROLLER_SHOW_PLAYER_SCREEN
} mp3_oled_controller__screen_update_e;

typedef struct {
  size_t current_top_song_index;
  size_t current_bottom_song_index;
  size_t highlighted_song_index;
  const mp3_s *mp3_playing_song;
  bool is_song_paused;
  uint8_t volume_percentage;
  uint8_t bass_level;
  uint8_t treble_level;
} mp3_oled_controller_s;

/**
 * Initialize MP3 OLED controller
 **/
void mp3_oled_controller__initialize(void);

/**
 * Show the song list screen
 **/
void mp3_oled_controller__song_list_show(void);

/**
 * Highlight a song in the list
 *
 * If the desired song is out of the screen, automatically scroll up or down.
 *
 * @param song_index is the index of the song to be highlighted
 **/
void mp3_oled_controller__song_list_highlight_song(size_t song_index);

/**
 * Show the player screen
 **/
void mp3_oled_controller__player_show(void);

/**
 * Unset the is_song_paused flag
 **/
void mp3_oled_controller__player_set_playing(void);

/**
 * Show the pause icon because there is a song playing currently
 **/
void mp3_oled_controller__player_show_playing(void);

/**
 * Set the is_song_paused flag
 **/
void mp3_oled_controller__player_set_paused(void);

/**
 * Show the play icon because there's no song currently playing
 **/
void mp3_oled_controller__player_show_paused(void);

/**
 * Set playing song
 * @param mp3_playing_song is the mp3 song that is playing
 **/
void mp3_oled_controller__player_set_playing_song(const mp3_s *const mp3_playing_song);

/**
 * Set volume
 * @param volume is the new volume level from 0 to 100
 **/
void mp3_oled_controller__player_set_volume(uint8_t volume);

/**
 * Set and show volume
 * @param volume is the new volume level from 0 to 100
 **/
void mp3_oled_controller__player_show_volume(uint8_t volume);

/**
 * Set bass
 * @param bass is the new bass level from 0 to 10
 **/
void mp3_oled_controller__player_set_bass(uint8_t bass);

/**
 * Set treble
 * @param treble is the new treble level from 0 to 10
 **/
void mp3_oled_controller__player_set_treble(uint8_t treble);

/**
 * Set and show bass
 * @param bass is the new bass level from 0 to 10
 **/
void mp3_oled_controller__player_show_bass(uint8_t bass);

/**
 * Set and show treble
 * @param treble is the new treble level from 0 to 10
 **/
void mp3_oled_controller__player_show_treble(uint8_t treble);

/**
 * Update the screen based on the current data values and flag statuses
 * @param update_id is the id of the type of update to be done
 **/
void mp3_oled_controller__update_screen(mp3_oled_controller__screen_update_e update_id);