#pragma once
#include <stdint.h>

void SSD1306_ascii_display_A();

void SSD1306_ascii_display_B();

void SSD1306_ascii_display_C();

void SSD1306_ascii_display_D();

void SSD1306_ascii_display_E();

void SSD1306_ascii_display_F();

void SSD1306_ascii_display_G();

void SSD1306_ascii_display_H();

void SSD1306_ascii_display_I();

void SSD1306_ascii_display_J();

void SSD1306_ascii_display_K();

void SSD1306_ascii_display_L();

void SSD1306_ascii_display_M();

void SSD1306_ascii_display_N();

void SSD1306_ascii_display_O();

void SSD1306_ascii_display_P();

void SSD1306_ascii_display_Q();

void SSD1306_ascii_display_R();

void SSD1306_ascii_display_S();

void SSD1306_ascii_display_T();

void SSD1306_ascii_display_U();

void SSD1306_ascii_display_V();

void SSD1306_ascii_display_W();

void SSD1306_ascii_display_X();

void SSD1306_ascii_display_Y();

void SSD1306_ascii_display_Z();

void SSD1306_ascii_display_0();

void SSD1306_ascii_display_1();

void SSD1306_ascii_display_2();

void SSD1306_ascii_display_3();

void SSD1306_ascii_display_4();

void SSD1306_ascii_display_5();

void SSD1306_ascii_display_6();

void SSD1306_ascii_display_7();

void SSD1306_ascii_display_8();

void SSD1306_ascii_display_9();

void SSD1306_ascii_display_space();

void SSD1306_ascii_display_driver(uint8_t ascii_letter_size, uint8_t ascii_letter[]);

void SSD1306_ascii_display_string(const char *const string);

void SSD1306_ascii_display_string_with_max_length(const char *const string_to_print, const uint32_t *const max_length);

void SSD1306_ascii_display_char(char character);
