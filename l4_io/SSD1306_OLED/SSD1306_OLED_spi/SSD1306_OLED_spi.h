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

#if 0
/**
 * Assert the reset pin. It is active low, so this means pull it to ground
 **/
void vs1053b__reset_assert();

/**
 * Deassert the reset pin. It is active low, so this maens pull it to VCC.
 **/
void vs1053b__reset_deeassert();





/**
 * Get the DREQ status
 **/
bool vs1053b__get_dreq();



/**
 * Receive a byte from the SPI bus
 **/
uint8_t vs1053b__receive_byte();

/**
 * A polling delay in milliseconds
 **/
void vs1053b__delay_ms(uint32_t ms);

#endif