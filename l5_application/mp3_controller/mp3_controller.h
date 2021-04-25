#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef enum {
  MP3_CONTROLLER__SHOW_SONGS = 0,
  MP3_CONTROLLER__SHOW_PLAYER,
  MP3_CONTROLLER__SCROLL_UP,
  MP3_CONTROLLER__SCROLL_DOWN,
  MP3_CONTROLLER__VOLUME_UP,
  MP3_CONTROLLER__VOLUME_DOWN,
  MP3_CONTROLLER__PLAY_SONG,
  MP3_CONTROLLER__PLAY_ENQUEUED_SONG,
  MP3_CONTROLLER__ENQUEUE_SONG,
  MP3_CONTROLLER__PAUSE_SONG,
  MP3_CONTROLLER__RESUME_SONG,
  MP3_CONTROLLER__STOP_SONG
} mp3_controller_controls_e;

typedef struct {
  mp3_controller_controls_e control;
  int argument;
} mp3_controller_s;

void mp3_controller__initialize(void);
void mp3_controller__scroll(const mp3_controller_s *const control);
size_t mp3_controller__get_scroll_index(void);
void mp3_controller__play_song(size_t item_number);
void mp3_controller__enqueue_song(size_t item_number);
void mp3_controller__pause_song(void);
void mp3_controller__resume_song(void);
void mp3_controller__stop_song(void);
void mp3_controller__play_enqueued_song(void);
void mp3_controller__go_to_player_screen(void);
void mp3_controller__go_to_song_list_screen(void);
void mp3_controller__reset_flags(void);
void mp3_controller__set_is_song_playing_flag(void);

bool mp3_controller__is_song_playing(void);
bool mp3_controller__is_song_paused(void);
bool mp3_controller__is_song_stopped(void);
bool mp3_controller__is_break_required(void);
bool mp3_controller__is_on_player_screen(void);

bool mp3_controller__execute_control(const mp3_controller_s *const control);