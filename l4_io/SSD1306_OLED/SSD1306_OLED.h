#pragma once

#include <stdbool.h>
#include <stdint.h>

bool SSD1306__initialize(void);

void SSD1306__write(uint8_t command, uint8_t data);
void SSD1306__command_write(uint8_t command, uint8_t *data, uint8_t size);
void SSD1306__data_write(uint8_t data);
void SSD1306__horizontalscroll();
void SSD1306__fadeout_on();
void SSD1306__zoom_in();
void SSD1306__delay_ms(uint32_t ms);

void SSD1306__alphabet_test(void);