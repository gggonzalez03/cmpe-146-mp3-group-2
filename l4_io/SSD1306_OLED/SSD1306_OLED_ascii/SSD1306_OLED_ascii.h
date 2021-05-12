#pragma once
#include <stdbool.h>
#include <stdint.h>

void SSD1306_ascii_display_A(bool negative);

void SSD1306_ascii_display_B(bool negative);

void SSD1306_ascii_display_C(bool negative);

void SSD1306_ascii_display_D(bool negative);

void SSD1306_ascii_display_E(bool negative);

void SSD1306_ascii_display_F(bool negative);

void SSD1306_ascii_display_G(bool negative);

void SSD1306_ascii_display_H(bool negative);

void SSD1306_ascii_display_I(bool negative);

void SSD1306_ascii_display_J(bool negative);

void SSD1306_ascii_display_K(bool negative);

void SSD1306_ascii_display_L(bool negative);

void SSD1306_ascii_display_M(bool negative);

void SSD1306_ascii_display_N(bool negative);

void SSD1306_ascii_display_O(bool negative);

void SSD1306_ascii_display_P(bool negative);

void SSD1306_ascii_display_Q(bool negative);

void SSD1306_ascii_display_R(bool negative);

void SSD1306_ascii_display_S(bool negative);

void SSD1306_ascii_display_T(bool negative);

void SSD1306_ascii_display_U(bool negative);

void SSD1306_ascii_display_V(bool negative);

void SSD1306_ascii_display_W(bool negative);

void SSD1306_ascii_display_X(bool negative);

void SSD1306_ascii_display_Y(bool negative);

void SSD1306_ascii_display_Z(bool negative);

void SSD1306_ascii_display_0(bool negative);

void SSD1306_ascii_display_1(bool negative);

void SSD1306_ascii_display_2(bool negative);

void SSD1306_ascii_display_3(bool negative);

void SSD1306_ascii_display_4(bool negative);

void SSD1306_ascii_display_5(bool negative);

void SSD1306_ascii_display_6(bool negative);

void SSD1306_ascii_display_7(bool negative);

void SSD1306_ascii_display_8(bool negative);

void SSD1306_ascii_display_9(bool negative);

void SSD1306_ascii_display_space(bool negative);

void SSD1306_ascii_display_driver(uint8_t ascii_letter_size, uint8_t ascii_letter[], bool negative);

void SSD1306_ascii_display_string(const char *const string);

void SSD1306_ascii_display_string_with_max_length(const char *const string_to_print, const uint32_t *const max_length,
                                                  bool negative);

void SSD1306_ascii_display_char(char character, bool negative);
