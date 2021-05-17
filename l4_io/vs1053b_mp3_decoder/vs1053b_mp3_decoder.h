#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  char header[10];
  char tag[50];
} vs1053b__mp3_song;

/**
 * Initialize the decoder
 * @return whether initialization is successful or not
 **/
bool vs1053b__mp3_decoder_initialize(void);

/**
 * @return whether the decoder can take more data or not
 **/
bool vs1053b__mp3_decoder_needs_data(void);

/**
 * Start the mp3 audio data transfer
 **/
void vs1053b__mp3_decoder_start();

/**
 * Transfer mp3 audio data
 **/
bool vs1053b__mp3_decoder_play_byte(uint8_t byte);

/**
 * End the mp3 audio data transfer
 **/
void vs1053b__mp3_decoder_end();

/**
 * Get the status of the decoderr (Status register value)
 **/
uint16_t vs1053b__get_status(void);

/**
 * Write data to a register on the vs1053b chip
 **/
void vs1053b__sci_write(uint8_t address, uint16_t data);

/**
 * Read data from a registerr on the vs1053b chip
 **/
uint16_t vs1053b__sci_read(uint8_t address);

/**
 * Soft reset the vs1053b chip
 **/
void vs1053b__soft_reset(void);

/**
 * Hard reset the vs1053b chip
 **/
void vs1053b__reset(void);

/**
 * Do a sine test on the vs1053b chip
 **/
void vs1053b__sine_test(uint8_t n, uint32_t duration_in_ms);

/**
 * Set volume
 * @param left is the volume for the left output in percent
 * @param right is the volume for the right output in percent
 **/
void vs1053b__set_volume(uint8_t left, uint8_t right);

/**
 * Set treble amplitude
 * @param treble_amp is to control amplitude of treble. ONLY first 0:3 bits valid
 **/
void vs1053b__set_treble_amplitude(int8_t treble_amp);

/**
 * Set treble frequency
 * @param treble_freq is to control frequency of treble. ONLY first 0:3 bits valid
 **/
void vs1053b__set_treble_frequency(uint8_t treble_freq);

/**
 * Set bass amplitude
 * @param bass_amp is to control amplitude of bass. ONLY first 0:3 bits valid
 **/
void vs1053b__set_bass_amplitude(uint8_t bass_amp);

/**
 * Set treble frequency
 * @param bass_freq is to control frequency of bass. ONLY first 0:3 bits valid
 **/
void vs1053b__set_bass_frequency(uint8_t bass_freq);
