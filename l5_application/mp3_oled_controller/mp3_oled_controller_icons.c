#include <stddef.h>

#include "SSD1306_OLED.h"
#include "SSD1306_OLED_ascii.h"
#include "SSD1306_OLED_spi.h"
#include "mp3_oled_controller_icons.h"

extern uint8_t mp3_oled_controller__max_lines_on_screen;
extern uint8_t mp3_oled_controller__oled_start_column;
extern uint8_t mp3_oled_controller__oled_end_column;

extern uint8_t mp3_oled_controller__oled_start_column_margin;
extern uint8_t mp3_oled_controller__oled_end_column_margin;

extern uint8_t mp3_oled_controller__oled_center_horizontal;

/************************************************************************************
 *
 *                                PRIVATE DECLARATIONS
 *
 ***********************************************************************************/

static const uint8_t nav_width = 0x0D;

/************************************************************************************
 *
 *                                  PRIVATE FUNCTIONS
 *
 ***********************************************************************************/
static void mp3_oled_controller_icons__print_left_arrow(uint8_t *start_page) {
  static const uint8_t triangle_width = nav_width;

  static uint8_t upper_triangle[0x0D] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  static uint8_t lower_triangle[0x0D] = {0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306__send_data(upper_triangle, triangle_width);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306__send_data(lower_triangle, triangle_width);
  (*start_page)++;
}

static void mp3_oled_controller_icons__print_right_arrow(uint8_t *start_page) {
  static const uint8_t triangle_width = nav_width;
  static uint8_t upper_triangle[0x0D] = {0x00, 0x00, 0x00, 0x00, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00};
  static uint8_t lower_triangle[0x0D] = {0x00, 0x00, 0x00, 0x00, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin, mp3_oled_controller__oled_end_column);
  SSD1306__send_data(upper_triangle, triangle_width);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin, mp3_oled_controller__oled_end_column);
  SSD1306__send_data(lower_triangle, triangle_width);
  (*start_page)++;
}

static void mp3_oled_controller_icons__print_enq_left_nav(uint8_t *start_page) {
  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306_ascii_display_E(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306_ascii_display_N(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306_ascii_display_Q(false);
  (*start_page)++;
}

static void mp3_oled_controller_icons__print_songs_left_nav(uint8_t *start_page) {
  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306_ascii_display_S(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306_ascii_display_0(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306_ascii_display_N(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306_ascii_display_G(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_start_column, mp3_oled_controller__oled_start_column_margin);
  SSD1306_ascii_display_S(false);
  (*start_page)++;
}

static void mp3_oled_controller_icons__print_player_right_nav(uint8_t *start_page) {
  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 4, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_P(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 4, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_L(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 4, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_A(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 4, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_Y(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 4, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_E(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 4, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_R(false);
  (*start_page)++;
}

static void mp3_oled_controller_icons__print_left_nav_divider() {
  static const uint8_t nav_divider_size = 3;
  static uint8_t nav_divider[0x03] = {0xFF, 0x00, 0x00};
  for (uint8_t page = 0; page < mp3_oled_controller__max_lines_on_screen; page++) {
    SSD1306__page_specify(page, page);
    SSD1306__column_specify(mp3_oled_controller__oled_start_column_margin - nav_divider_size,
                            mp3_oled_controller__oled_start_column_margin);
    SSD1306__send_data(nav_divider, nav_divider_size);
  }
}

static void mp3_oled_controller_icons__print_right_nav_divider() {
  static const uint8_t nav_divider_size = 3;
  static uint8_t nav_divider[0x03] = {0x00, 0x00, 0xFF};

  for (size_t page = 0; page < mp3_oled_controller__max_lines_on_screen; page++) {
    SSD1306__page_specify(page, page);
    SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin,
                            mp3_oled_controller__oled_end_column - nav_width - nav_divider_size);
    SSD1306__send_data(nav_divider, nav_divider_size);
  }
}

/************************************************************************************
 *
 *                                   PUBLIC FUNCTIONS
 *
 ***********************************************************************************/

void mp3_oled_controller_icons__print_left_nav(bool is_on_player_screen) {
  uint8_t page = 0UL;
  mp3_oled_controller_icons__print_left_arrow(&page);

  if (is_on_player_screen) {
    mp3_oled_controller_icons__print_songs_left_nav(&page);
  } else {
    mp3_oled_controller_icons__print_enq_left_nav(&page);
  }
  mp3_oled_controller_icons__print_left_nav_divider();
}

void mp3_oled_controller_icons__print_right_nav(bool is_on_player_screen) {
  uint8_t page = 0UL;
  mp3_oled_controller_icons__print_right_arrow(&page);

  if (!is_on_player_screen) {
    mp3_oled_controller_icons__print_player_right_nav(&page);
  }
  mp3_oled_controller_icons__print_right_nav_divider();
}

void mp3_oled_controller_icons__print_play_icon(void) {
  uint8_t start_page = 4;

  static const uint8_t triangle_width = 7;
  const uint8_t start_column = mp3_oled_controller__oled_center_horizontal;
  const uint8_t end_column = start_column + triangle_width;

  static uint8_t upper_triangle[7] = {0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80};
  static uint8_t lower_triangle[7] = {0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00};

  SSD1306__page_specify(start_page, start_page);
  SSD1306__column_specify(start_column, end_column);
  SSD1306__send_data(upper_triangle, triangle_width);
  start_page++;

  SSD1306__page_specify(start_page, start_page);
  SSD1306__column_specify(start_column, end_column);
  SSD1306__send_data(lower_triangle, triangle_width);
}

void mp3_oled_controller_icons__print_pause_icon(void) {
  uint8_t start_page = 4;

  static const uint8_t pause_icon_width = 8;
  const uint8_t start_column = mp3_oled_controller__oled_center_horizontal;
  const uint8_t end_column = start_column + pause_icon_width;

  static uint8_t upper_squares[8] = {0xFE, 0xFE, 0xFE, 0x00, 0x00, 0xFE, 0xFE, 0xFE};
  static uint8_t lower_squares[8] = {0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x7F, 0x7F, 0x7F};

  SSD1306__page_specify(start_page, start_page);
  SSD1306__column_specify(start_column, end_column);
  SSD1306__send_data(upper_squares, pause_icon_width);
  start_page++;

  SSD1306__page_specify(start_page, start_page);
  SSD1306__column_specify(start_column, end_column);
  SSD1306__send_data(lower_squares, pause_icon_width);
}

void mp3_oled_controller_icons__print_next_song_icon(void) {
  uint8_t start_page = 4;

  static const uint8_t next_icon_width = 10;
  const uint8_t column_offset_from_center = 20;
  const uint8_t start_column = mp3_oled_controller__oled_center_horizontal + column_offset_from_center;
  const uint8_t end_column = start_column + next_icon_width;

  static uint8_t upper_triangles[10] = {0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0xF8, 0xF0, 0xE0, 0xC0, 0x80};
  static uint8_t lower_triangles[10] = {0x0F, 0x07, 0x03, 0x01, 0x00, 0x0F, 0x07, 0x03, 0x01, 0x00};

  SSD1306__page_specify(start_page, start_page);
  SSD1306__column_specify(start_column, end_column);
  SSD1306__send_data(upper_triangles, next_icon_width);
  start_page++;

  SSD1306__page_specify(start_page, start_page);
  SSD1306__column_specify(start_column, end_column);
  SSD1306__send_data(lower_triangles, next_icon_width);
}

void mp3_oled_controller_icons__print_volume_bar_icon(uint8_t level) {
  uint8_t start_page = 7;

  static const uint8_t volume_bar_width = 100;
  static const uint8_t volume_circle_width = 5;
  const uint8_t start_column = mp3_oled_controller__oled_start_column_margin + 1;
  const uint8_t end_column = mp3_oled_controller__oled_end_column_margin - 1;

  static uint8_t volume_bar_line = 0x18;
  static uint8_t volume_circle[5] = {0x3C, 0x7E, 0x7E, 0x7E, 0x3C};
  static uint8_t volume_plus_sign[6] = {0x18, 0x18, 0x7E, 0x7E, 0x18, 0x18};
  static uint8_t volume_speaker_sign[6] = {0x3C, 0x3C, 0x3C, 0x7E, 0xFF, 0xFF};
  static uint8_t volume_minus_sign[6] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x18};

  const uint8_t start_page_speaker_logo = 0x06;
  const uint8_t end_column_speaker_logo_minus = 0x1B;
  const uint8_t start_column_speaker_logo_plus = 0x62;

  SSD1306__page_specify(start_page_speaker_logo, start_page_speaker_logo);
  SSD1306__column_specify(start_column, end_column_speaker_logo_minus);
  SSD1306__send_data(volume_speaker_sign, 6);
  SSD1306_ascii_display_space(false);
  SSD1306__send_data(volume_minus_sign, 5);

  SSD1306__column_specify(start_column_speaker_logo_plus, end_column);
  SSD1306__send_data(volume_speaker_sign, 6);
  SSD1306_ascii_display_space(false);
  SSD1306__send_data(volume_plus_sign, 6);

  uint8_t volume_circle_start_index = 0;

  // level 5 is the lowest we can show
  if (level < volume_circle_width) {
    level = 5;
  }

  SSD1306__page_specify(start_page, start_page);
  SSD1306__column_specify(start_column, end_column);

  SSD1306__cs();
  SSD1306__data_cs();
  for (int index = 0; index < volume_bar_width; index++) {
    if (index >= (level - volume_circle_width) && volume_circle_start_index < volume_circle_width) {
      SSD1306__transmit_byte(volume_circle[volume_circle_start_index]);
      volume_circle_start_index++;
      continue;
    }

    SSD1306__transmit_byte(volume_bar_line);
  }
  SSD1306__data_ds();
  SSD1306__ds();
}

void mp3_oled_controller_icons__print_treble_bar_icon(int treb_level) {
  uint8_t treble_index = 0;

  static uint8_t treble_bar[4] = {0x40, 0x50, 0x54, 0x55};

  SSD1306__column_specify(0x75, 0x7F);
  SSD1306__page_specify(0x03, 0x03);
  SSD1306_ascii_display_T(false);

  SSD1306__page_specify(0x02, 0x02);

  for (int index = 1; index < treb_level + 1; index++) {

    if (index == 5) {
      SSD1306__page_specify(0x01, 0x01);
      treble_index = 0;

    } else if (index == 9) {
      SSD1306__page_specify(0x00, 0x00);
      treble_index = 0;
    } else {
      ;
    }
    SSD1306__column_specify(0x76, 0x7F);
    SSD1306__cs();
    SSD1306__data_cs();
    SSD1306__transmit_byte(treble_bar[treble_index]);
    SSD1306__transmit_byte(treble_bar[treble_index]);
    SSD1306__transmit_byte(treble_bar[treble_index]);
    SSD1306__transmit_byte(treble_bar[treble_index]);
    SSD1306__data_ds();
    SSD1306__ds();
    treble_index++;
  }
}

void mp3_oled_controller_icons__print_bass_bar_icon(int bass_level) {
  uint8_t bass_index = 0;

  static uint8_t bass_bar[4] = {0x40, 0x50, 0x54, 0x55};

  SSD1306__column_specify(0x75, 0x7F);
  SSD1306__page_specify(0x07, 0x07);
  SSD1306_ascii_display_B(false);

  SSD1306__page_specify(0x06, 0x06);

  for (int index = 1; index < bass_level + 1; index++) {

    if (index == 5) {
      SSD1306__page_specify(0x05, 0x05);
      bass_index = 0;

    } else if (index == 9) {
      SSD1306__page_specify(0x04, 0x04);
      bass_index = 0;
    } else {
      ;
    }
    SSD1306__column_specify(0x76, 0x7F);
    SSD1306__cs();
    SSD1306__data_cs();
    SSD1306__transmit_byte(bass_bar[bass_index]);
    SSD1306__transmit_byte(bass_bar[bass_index]);
    SSD1306__transmit_byte(bass_bar[bass_index]);
    SSD1306__transmit_byte(bass_bar[bass_index]);
    SSD1306__data_ds();
    SSD1306__ds();
    bass_index++;
  }
}
