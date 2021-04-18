#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  char header[10];
  char tag[50];
} vs1053b__mp3_song;

bool vs1053b__mp3_decoder_initialize(void);

bool vs1053b__mp3_decoder_needs_data(void);

bool vs1053b__mp3_decoder_play_byte(uint8_t byte);

uint16_t vs1053b__get_status(void);

void vs1053b__sci_write(uint8_t address, uint16_t data);

uint16_t vs1053b__sci_read(uint8_t address);

void vs1053b__soft_reset(void);

void vs1053b__reset(void);

void vs1053b__sine_test(uint8_t n, uint32_t duration_in_ms);