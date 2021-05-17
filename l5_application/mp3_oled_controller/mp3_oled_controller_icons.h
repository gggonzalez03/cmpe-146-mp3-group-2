#pragma once

void mp3_oled_controller_icons__print_left_nav(bool is_on_player_screen);
void mp3_oled_controller_icons__print_right_nav(bool is_on_player_screen);
void mp3_oled_controller_icons__print_play_icon(void);
void mp3_oled_controller_icons__print_pause_icon(void);
void mp3_oled_controller_icons__print_next_song_icon(void);
void mp3_oled_controller_icons__print_volume_bar_icon(uint8_t level);

/**
 * Update the treble bar in the screen based on its current value
 * @param treb_level takes values from 1 to 8
 **/
void mp3_oled_controller_icons__print_treble_bar_icon(int treb_level);

/**
 * Update the treble bar in the screen based on its current value
 * @param bass_level takes values from 1 to 8
 **/
void mp3_oled_controller_icons__print_bass_bar_icon(int bass_level);
