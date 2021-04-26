#include "SSD1306_OLED.h"
#include "SSD1306_OLED_ascii.h"
#include "SSD1306_OLED_spi.h"

#include "delay.h"

// static bool command= FALSE ;

bool SSD1306__initialize(void) {
  SSD1306__configure_spi();
  delay__ms(100);
  return true;
}

void SSD1306__write(uint8_t command, uint8_t data) {

  SSD1306__cs();
  SSD1306__data_ds();
  SSD1306__transmit_byte(command);

  if (data != 0) {
    SSD1306__transmit_byte(data);
  }
  SSD1306__ds();
}

void SSD1306__command_write(uint8_t command, uint8_t *data, uint8_t size) {
  SSD1306__cs();
  SSD1306__data_ds();

  SSD1306__transmit_byte(command);

  for (uint8_t index = 0; index < size; index++) {
    SSD1306__transmit_byte(data[index]);
  }

  SSD1306__ds();
}

void SSD1306__horizontalscroll_on() {
  SSD1306__cs();
  SSD1306__data_ds();

  SSD1306__transmit_byte(0x26); // command to setup horizontal scroll
  SSD1306__transmit_byte(0x00); // dummy byte
  SSD1306__transmit_byte(0x00); // define start page address
  SSD1306__transmit_byte(0x07); // time interval between each scrolll
  SSD1306__transmit_byte(0x00); // define page end address
  SSD1306__transmit_byte(0x00); // dummy byte
  SSD1306__transmit_byte(0xFF); // dummy byte

  SSD1306__transmit_byte(0x2F); // activate scrolling
  SSD1306__ds();
}

void SSD1306__data_write(uint8_t data) {
  SSD1306__cs();
  SSD1306__data_cs();
  SSD1306__transmit_byte(data);
  SSD1306__data_ds();
  SSD1306__ds();
}

void SSD1306__delay_ms(uint32_t ms) { delay__ms(ms); }

void SSD1306__alphabet_test(void) {
  SSD1306__write(0xAE, 0);
  SSD1306__write(0xD5, 0x80);
  SSD1306__write(0xA8, 0x3F);
  SSD1306__write(0xD3, 0x20); // set display offset
  SSD1306__write(0x40, 0);
  SSD1306__write(0x8D, 0x14);
  SSD1306__write(0xA1, 0);
  SSD1306__write(0xC8, 0);
  SSD1306__write(0xDA, 0x12);
  SSD1306__write(0x81, 0xCF);
  SSD1306__write(0xD9, 0xF1);
  SSD1306__write(0xDB, 0x40);
  SSD1306__write(0xA4, 0);
  SSD1306__write(0xA6, 0);
  SSD1306__write(0xAF, 0);
  // SSD1306__write(0xA5, 0);
  SSD1306__write(0x20, 0x00);
  // SSD1306__write(0x21, 0);

  SSD1306__write(0xA4, 0);

  uint8_t column_data[] = {0x00, 0x7F};
  uint8_t page_data[] = {0x00, 0x07};

  SSD1306__command_write(0x21, column_data, 2);
  SSD1306__command_write(0x22, page_data, 2);

  for (int i = 0; i < 1024; i++) {
    SSD1306__data_write(0x00);
  }

  SSD1306_ascii_display_A();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_B();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_C();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_D();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_E();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_F();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_G();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_H();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_I();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_J();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_K();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_L();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_M();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_N();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_O();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_P();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_Q();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_R();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_S();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_T();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_U();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_V();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_W();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_X();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_Y();
  SSD1306_ascii_display_space();

  SSD1306_ascii_display_Z();
  SSD1306_ascii_display_space();

  // Scrolling command //
  // SSD1306__horizontalscroll_on();

  // scrolling end //
}