#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "SSD1306_OLED.h"
#include "SSD1306_OLED_ascii.h"

#include "mp3_song_list.h"

typedef struct {
  size_t current_top_song_index;
  size_t current_bottom_song_index;
  size_t highlighted_song_index;
  size_t playing_song_index;
  size_t max_lines_on_screen;
  bool is_song_playing;
  bool is_song_paused;
  bool is_on_player_screen;
} mp3_oled_controller_s;

/**
 * Initialize MP3 OLED controller
 **/
void mp3_oled_controller__initialize(void);

/**
 * Show the song list screen
 **/
void mp3_oled_controller__song_list_show();

/**
 * Highlight a song in the list
 *
 * If the desired song is out of the screen, automatically scroll up or down.
 *
 * @param song_index is the index of the song to be highlighted
 **/
void mp3_oled_controller__song_list_highlight_song(size_t song_index);

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