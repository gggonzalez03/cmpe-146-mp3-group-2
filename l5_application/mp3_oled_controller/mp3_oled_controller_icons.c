#include <stddef.h>

#include "SSD1306_OLED.h"
#include "SSD1306_OLED_ascii.h"
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

static const uint8_t nav_width = 0x0F;

/************************************************************************************
 *
 *                                  PRIVATE FUNCTIONS
 *
 ***********************************************************************************/
static void mp3_oled_controller_icons__print_left_arrow(uint8_t *start_page) {
  static const uint8_t triangle_width = nav_width;

  static uint8_t upper_triangle[0x0F] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  static uint8_t lower_triangle[0x0F] = {0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

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
  static uint8_t upper_triangle[0x0F] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00};
  static uint8_t lower_triangle[0x0F] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00};

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
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 8, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_P(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 8, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_L(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 8, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_A(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 8, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_Y(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 8, mp3_oled_controller__oled_end_column);
  SSD1306_ascii_display_E(false);
  (*start_page)++;

  SSD1306__page_specify(*start_page, *start_page);
  SSD1306__column_specify(mp3_oled_controller__oled_end_column_margin + 8, mp3_oled_controller__oled_end_column);
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
  // uint8_t start_page = 4;

  // static const uint8_t triangle_width = 8;
  // const uint8_t start_column = mp3_oled_controller__oled_center_horizontal;
  // const uint8_t end_column = start_column + triangle_width;

  // static uint8_t upper_triangle[8] = {0xFE, 0xFE, 0xFE, 0x00, 0x00, 0xFE, 0xFE, 0xFE};
  // static uint8_t lower_triangle[8] = {0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x7F, 0x7F, 0x7F};

  // SSD1306__page_specify(start_page, start_page);
  // SSD1306__column_specify(start_column, end_column);
  // SSD1306__send_data(upper_triangle, triangle_width);
  // start_page++;

  // SSD1306__page_specify(start_page, start_page);
  // SSD1306__column_specify(start_column, end_column);
  // SSD1306__send_data(lower_triangle, triangle_width);
}