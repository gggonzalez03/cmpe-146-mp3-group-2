#include "SSD1306_OLED.h"
#include "SSD1306_OLED_ascii.h"
#include "SSD1306_OLED_spi.h"
#include "delay.h"
#include <stddef.h>

bool SSD1306__initialize(void) {
  SSD1306__configure_spi();
  SSD1306__display_initialize();
  SSD1306__clear_screen();
  return true;
}

void SSD1306__send_data(uint8_t *data, int size) {
  SSD1306__cs();

  for (int index = 0; index < size; index++) {
    SSD1306__data_cs();
    SSD1306__transmit_byte(data[index]);
    SSD1306__data_ds();
  }
  SSD1306__ds();
}

void SSD1306__send_command(uint8_t *command, int size) {
  SSD1306__cs();
  SSD1306__data_ds();
  for (int index = 0; index < size; index++) {
    SSD1306__transmit_byte(command[index]);
  }
  SSD1306__ds();
}

void SSD1306__display_initialize(void) {
  // TODO: use SSD1306__command_write() instead

  uint8_t display_off[1] = {0xAE};
  uint8_t display_clock_frequency[2] = {0xD5, 0x80};
  uint8_t multiplex_ratio[2] = {0xA8, 0x3F};
  uint8_t display_offset[2] = {0xD3, 0x00};
  uint8_t start_line[1] = {0x40};
  uint8_t set_charge_pump[2] = {0x8D, 0x14};
  uint8_t segment_remap[1] = {0xA1};
  uint8_t com_output_direction[1] = {0xC8};
  uint8_t pins_hardware_config[2] = {0xDA, 0x12};
  uint8_t contrast_control[2] = {0x81, 0xCF};
  uint8_t precharge_period[2] = {0xD9, 0xF1};
  uint8_t vcomh_deselect[2] = {0xDB, 0x40};
  uint8_t normal_inverse_display[1] = {0xA6};
  uint8_t display_on[1] = {0xAF};
  uint8_t set_memory_address_mode[2] = {0x20, 0x00};
  uint8_t entire_display_onoff[1] = {0xA4};

  SSD1306__send_command(display_off, 1);
  SSD1306__send_command(display_clock_frequency, 2);
  SSD1306__send_command(multiplex_ratio, 2);
  SSD1306__send_command(display_offset, 2);
  SSD1306__send_command(start_line, 1);
  SSD1306__send_command(set_charge_pump, 2);
  SSD1306__send_command(segment_remap, 1);
  SSD1306__send_command(com_output_direction, 1);
  SSD1306__send_command(pins_hardware_config, 2);
  SSD1306__send_command(contrast_control, 2);
  SSD1306__send_command(precharge_period, 2);
  SSD1306__send_command(vcomh_deselect, 2);
  SSD1306__send_command(normal_inverse_display, 1);
  SSD1306__send_command(display_on, 1);
  SSD1306__send_command(set_memory_address_mode, 2);
  SSD1306__send_command(entire_display_onoff, 1);
}

void SSD1306__clear_screen(void) {

  uint8_t first_column = 0x00;
  uint8_t first_page = 0x00;
  uint8_t end_column = 0x7F;
  uint8_t end_page = 0x07;

  SSD1306__page_specify(first_page, end_page);
  SSD1306__column_specify(first_column, end_column);

  // writing all pages and columns as 0x00
  uint8_t all_pages_columns[1024] = {0x00};
  SSD1306__send_data(all_pages_columns, 1024);
}

void SSD1306__page_specify(uint8_t start_location_command, uint8_t end_location_command) {
  uint8_t page_command = 0x22;
  uint8_t p_commands[3] = {page_command, start_location_command, end_location_command};
  SSD1306__send_command(p_commands, 3);
}
void SSD1306__column_specify(uint8_t start_location_command, uint8_t end_location_command) {
  uint8_t column_command = 0x21;
  uint8_t c_commands[3] = {column_command, start_location_command, end_location_command};
  SSD1306__send_command(c_commands, 3);
}

void SSD1306__horizontalscroll_on(uint8_t start_page, uint8_t end_page) {
  uint8_t dummy_byte = 0x00;
  uint8_t setup_horizontall_scroll = 0x00;
  uint8_t time_interval = 0x07;
  uint8_t horizontal_scroll_commands[7] = {
      setup_horizontall_scroll, dummy_byte, start_page, time_interval, end_page, dummy_byte, dummy_byte};
  uint8_t activate_scrolling[1] = {0x2F};

  SSD1306__send_command(horizontal_scroll_commands, 7);
  SSD1306__send_command(activate_scrolling, 1);
}

void SSD1306__fadeout_on() {
  uint8_t fade_out = 0x23;
  uint8_t enable_fadeout_mode = 0x21;
  uint8_t fade_commands[2] = {fade_out, enable_fadeout_mode};
  SSD1306__send_command(fade_commands, 2);
}

void SSD1306__delay_ms(uint32_t ms) { delay__ms(ms); }

////////////////////////////  TEST FUNCTIONS //////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////

void SSD1306__displaymenu_test1(void) {

  // specifying location address
  SSD1306__page_specify(0x00, 0x00);
  SSD1306__column_specify(0x20, 0x7F);

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_U();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_R();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_M();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_U();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_I();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_C();
  SSD1306_ascii_display_space();

  /////////////

  SSD1306__page_specify(0x01, 0x01);
  SSD1306__column_specify(0x24, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_R();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  ////////////

  SSD1306__page_specify(0x02, 0x01);
  SSD1306__column_specify(0x24, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_P();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_A();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_C();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_H();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  //

  SSD1306__page_specify(0x03, 0x01);
  SSD1306__column_specify(0x20, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_G();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_D();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_D();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_A();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_Y();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  ///

  SSD1306__page_specify(0x04, 0x01);
  SSD1306__column_specify(0x24, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_Y();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_L();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_L();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_W();
  SSD1306_ascii_display_space();

  ///

  SSD1306__page_specify(0x05, 0x01);
  SSD1306__column_specify(0x24, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_B();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_R();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_A();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_T();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_H();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();
}

//////////////////////////////////////////
void SSD1306__displaymenu_test2(void) {
  SSD1306__clear_screen();
  // specifying location address
  SSD1306__page_specify(0x00, 0x00);
  SSD1306__column_specify(0x20, 0x7F);

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_U();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_R();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_M();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_U();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_I();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_C();
  SSD1306_ascii_display_space();

  /////////////

  ////////////

  SSD1306__page_specify(0x01, 0x01);
  SSD1306__column_specify(0x24, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_P();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_A();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_C();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_H();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  //

  SSD1306__page_specify(0x02, 0x01);
  SSD1306__column_specify(0x20, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_G();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_D();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_D();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_A();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_Y();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  ///

  SSD1306__page_specify(0x03, 0x01);
  SSD1306__column_specify(0x24, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_Y();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_L();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_L();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_W();
  SSD1306_ascii_display_space();

  ///

  SSD1306__page_specify(0x04, 0x01);
  SSD1306__column_specify(0x24, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_B();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_R();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_A();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_T();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_H();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306__page_specify(0x05, 0x01);
  SSD1306__column_specify(0x24, 0x7F); // need to specify last column of that row

  SSD1306_ascii_display_R();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();
}
