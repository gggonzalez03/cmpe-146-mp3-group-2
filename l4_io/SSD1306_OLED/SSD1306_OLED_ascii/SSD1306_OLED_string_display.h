#pragma once
#include <stdint.h>
#include <stdio.h>

typedef struct {
  char *string_content;
  uint32_t string_size;
} SSD1306_OLED_string;

void SSD1306_ascii_display_string(SSD1306_OLED_string string);

void SSD1306_ascii_display_char(char character);