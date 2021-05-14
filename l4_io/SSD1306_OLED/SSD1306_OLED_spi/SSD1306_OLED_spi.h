#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Configure the SPI peripheral where the  OLED display is connected to
 **/
void SSD1306__configure_spi();

/**
 * Chip select the OLED display for Serial Command Interfacing
 * This means that command byte, and data words are sent through the bus
 **/
void SSD1306__cs();
void SSD1306__ds(); // chip deselect

void SSD1306__rst_high();
void SSD1306__rst_low();
/**
 * Chip select the OLED display for Serial Data Interfacing
 * This means that only data bytes are transfered. They go straight
 * to the OLED display for decoding
 **/
void SSD1306__data_cs(); // sets D/C# as low = writes commmand
void SSD1306__data_ds(); // sets D/C# as high = writes data

/**
 * Transmit a byte on the SPI bus
 **/
void SSD1306__transmit_byte(uint8_t byte);