#include "vs1053b_mp3_decoder.h"
#include "vs1053b_mp3_decoder_spi.h"

bool vs1053b__mp3_decoder_initialize(void) {
  vs1053b__configure_spi(1);
  vs1053b__reset();

  /**
   * Now that SCI_CLOCKF has been updated for a higher CLKI speed (in reset),
   * We can reconfigure SPI for a higher speed too, up to CLKI/4.
   * For us that is:
   * (CLKI / 4) = (XTAL * 3.5 / 4) = (12 * 3.5 / 4) = (10.5MHz)
   *
   * Note that if we were using SCI commands during operation, that
   * divider 4 would have been 7, which would have resulteed in a
   * slower max speed.
   *
   * Refer to Chapter 4.5, Note 2.
   **/
  vs1053b__configure_spi(8);
  return true;
}

bool vs1053b__mp3_decoder_needs_data(void) { return vs1053b__get_dreq(); }

void vs1053b__mp3_decoder_start() { vs1053b__dcs(); }

bool vs1053b__mp3_decoder_play_byte(uint8_t byte) {
  vs1053b__transmit_byte(byte);
  return true;
}

void vs1053b__mp3_decoder_end() { vs1053b__dds(); }

uint16_t vs1053b__get_status(void) {

  uint8_t status_register_address = 0x01;

  uint16_t status = 0x00;

  status = vs1053b__sci_read(status_register_address);

  return status;
}

void vs1053b__sci_write(uint8_t address, uint16_t data) {
  uint8_t write_op_code = 0x02;

  vs1053b__cs();
  vs1053b__transmit_byte(write_op_code);
  vs1053b__transmit_byte(address);
  vs1053b__transmit_byte(data >> 8);
  vs1053b__transmit_byte(data & 0xFF);
  vs1053b__ds();
}

uint16_t vs1053b__sci_read(uint8_t address) {
  uint8_t read_op_code = 0x03;
  uint16_t data = 0x00;

  vs1053b__cs();
  vs1053b__transmit_byte(read_op_code);
  vs1053b__transmit_byte(address);
  data |= ((uint16_t)vs1053b__receive_byte() << 8) | ((uint16_t)vs1053b__receive_byte());
  vs1053b__ds();

  return data;
}

void vs1053b__soft_reset(void) {
  vs1053b__sci_write(0x00, 0x0804);
  vs1053b__delay_ms(10);
}

/**
 * For delay requirements, refer to Chapter 9.6 of the datasheet.
 **/
void vs1053b__reset(void) {

  uint8_t sci_clockf_register = 0x03;
  uint16_t mult_3p5x_add_1p0 = 0x8800; // Multiplier 3.5x and Add XTALI * 1.0

  vs1053b__reset_assert();
  vs1053b__delay_ms(10);
  vs1053b__reset_deeassert();

  vs1053b__ds();
  vs1053b__dds();

  // Datasheet requires at least 1.8ms delay after hardware reset
  // before any further operations
  vs1053b__delay_ms(10);
  vs1053b__soft_reset();
  // Datasheet requires at least 1.8ms delay after software reset
  // before any further operations
  vs1053b__delay_ms(10);

  vs1053b__sci_write(sci_clockf_register, mult_3p5x_add_1p0);
  vs1053b__delay_ms(100);
}

void vs1053b__sine_test(uint8_t n, uint32_t duration_in_ms) {
  uint8_t mode_register = 0x00;
  uint16_t allow_test_mode = 0x0020;

  uint16_t current_mode = vs1053b__sci_read(mode_register);

  vs1053b__reset();

  vs1053b__sci_write(mode_register, current_mode | allow_test_mode);

  while (!vs1053b__get_dreq())
    ;

  // 8-byte sequence to initialize sine test
  vs1053b__dcs();
  vs1053b__transmit_byte(0x53);
  vs1053b__transmit_byte(0xEF);
  vs1053b__transmit_byte(0x6E);
  vs1053b__transmit_byte(n);
  vs1053b__transmit_byte(0x00);
  vs1053b__transmit_byte(0x00);
  vs1053b__transmit_byte(0x00);
  vs1053b__transmit_byte(0x00);
  vs1053b__dds();

  vs1053b__delay_ms(duration_in_ms);

  // 8-byte sequence to exit sine test
  vs1053b__dcs();
  vs1053b__transmit_byte(0x45);
  vs1053b__transmit_byte(0x78);
  vs1053b__transmit_byte(0x69);
  vs1053b__transmit_byte(0x74);
  vs1053b__transmit_byte(0x00);
  vs1053b__transmit_byte(0x00);
  vs1053b__transmit_byte(0x00);
  vs1053b__transmit_byte(0x00);
  vs1053b__dds();
}

void vs1053b__set_volume(uint8_t left, uint8_t right) {
  uint8_t volume_command = 0x0B;

  uint8_t minimum_volume = 0xC8; // 200d
  uint16_t non_normalized_left = minimum_volume - ((uint16_t)left << 1);
  uint16_t non_normalized_right = minimum_volume - ((uint16_t)right << 1);

  uint16_t left_and_right = ((uint16_t)non_normalized_left << 8) | (uint16_t)non_normalized_right;

  while (!vs1053b__get_dreq())
    ;

  // Deselect data chip select first before transmitting change volume command
  // This will solve the glitch issues when changing the volume while a song
  // is playing
  vs1053b__dds();
  vs1053b__sci_write(volume_command, left_and_right);
  vs1053b__dcs(); // Reselect data chip select, continue playing song
}