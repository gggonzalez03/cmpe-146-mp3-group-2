#pragma once

#include <stdbool.h>
#include <stdint.h>

bool SSD1306__initialize(void);

void SSD1306__send_data(uint8_t *data, int size); // This stays

void SSD1306__send_command(uint8_t *command, int size);

void SSD1306__display_initialize(void);

void SSD1306__clear_screen(void);

void SSD1306__page_specify(uint8_t start_location_command, uint8_t end_location_command);

void SSD1306__column_specify(uint8_t start_location_command, uint8_t end_location_command);

void SSD1306__horizontalscroll();

void SSD1306__fadeout_on();

void SSD1306__delay_ms(uint32_t ms);

/// Test Functions ////
void SSD1306__displaymenu_test1(void);

void SSD1306__displaymenu_test2(void);
