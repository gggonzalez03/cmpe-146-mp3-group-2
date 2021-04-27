#pragma once

#include <stdbool.h>
#include <stdint.h>

bool SSD1306__initialize(void);

void SSD1306__write(uint8_t command, uint8_t data); // Needs to go

void SSD1306__command_write(uint8_t command, uint8_t *data, uint8_t size); // This stays
void SSD1306__data_write(uint8_t data);

void SSD1306__display_on(void);
void SSD1306__clear_screen(void);

void SSD1306__set_starting_row(uint8_t row);
void SSD1306__set_starting_column(uint8_t column);

// Need to be updated once SSD1306__set_starting_row and SSD1306__set_starting_column work
void SSD1306__horizontalscroll();
void SSD1306__fadeout_on();
void SSD1306__zoom_in();

void SSD1306__delay_ms(uint32_t ms);
void SSD1306__alphabet_test(void);