#include "SSD1306_OLED.h"
#include "SSD1306_OLED_spi.h"
#include <stdio.h>

// static bool command= FALSE ;

bool SSD1306_OLED_initialize(void) {
  SSD1306__configure_spi();
  //  vs1053b__reset();
  return true;
}

void SSD1306__write(uint8_t command, uint8_t data) {

  SSD1306__cs();
  SSD1306__data_ds();
  SSD1306__transmit_byte(command);

  if (data != NULL) {
    SSD1306__transmit_byte(data);
  }
  SSD1306__ds();
}

SSD1306__command_write(uint8_t command, uint8_t *data, uint8_t size) {
  SSD1306__cs();
  SSD1306__data_ds();

  SSD1306__transmit_byte(command);

  for (uint8_t index = 0; index < size; index++) {
    SSD1306__transmit_byte(data[index]);
  }

  SSD1306__ds();
}

void SSD1306__data_write(uint8_t data) {
  SSD1306__cs();
  SSD1306__data_cs();
  SSD1306__transmit_byte(data);
  SSD1306__data_ds();
  SSD1306__ds();
}

#if 0

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
  vs1053b__sci_write(0x00, 0x0804); // 0000 1000 0000 0100 native spi mode and soft reset? 
  vs1053b__delay_ms(100);
}

void vs1053b__reset(void) {
  vs1053b__reset_assert();
  vs1053b__delay_ms(100); // is this delay for dreq to go low fo 22000 cycles/1.8ms? how does dreq goes to low ?
  vs1053b__reset_deeassert();

  vs1053b__ds();
  vs1053b__dds();

  vs1053b__delay_ms(100);
  vs1053b__soft_reset();
  vs1053b__delay_ms(100);

  vs1053b__sci_write(0x03, 0x6000); //   XTALI×3.0?

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

#endif