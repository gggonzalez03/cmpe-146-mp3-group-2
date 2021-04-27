#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Configure the SPI peripheral where the MP3 decoder is connected to
 **/
void vs1053b__configure_spi(uint32_t spi_clock_mhz);

/**
 * Assert the reset pin. It is active low, so this means pull it to ground
 **/
void vs1053b__reset_assert();

/**
 * Deassert the reset pin. It is active low, so this maens pull it to VCC.
 **/
void vs1053b__reset_deeassert();

/**
 * Chip select the MP3 decoder for Serial Command Interfacing
 * This means that instruction byte, address byte, and data words are
 * sent through the bus
 **/
void vs1053b__cs();
void vs1053b__ds(); // chip deselect

/**
 * Chip select the MP3 decoderr for Serial Data Interfacing
 * This means that only data bytes are transfered. They go straight
 * to the MP3 decoder for decoding
 **/
void vs1053b__dcs();
void vs1053b__dds(); // data chip deselect

/**
 * Get the DREQ status
 **/
bool vs1053b__get_dreq();

/**
 * Transmit a byte on the SPI bus
 **/
void vs1053b__transmit_byte(uint8_t byte);

/**
 * Receive a byte from the SPI bus
 **/
uint8_t vs1053b__receive_byte();

/**
 * A polling delay in milliseconds
 **/
void vs1053b__delay_ms(uint32_t ms);