#include "SSD1306_OLED_spi.h"
#include "delay.h"
#include "gpio.h"
#include "ssp1.h"

static gpio_s SSD1306_cs_pin = {GPIO__PORT_1, 22};

static gpio_s SSD1306_dcs_pin = {GPIO__PORT_1, 25};

// static gpio_s vs1053b_rst_pin = {GPIO__PORT_2, 6};

// static gpio_s vs1053b_dreq_pin = {GPIO__PORT_0, 16};

void SSD1306__configure_spi() {

  const uint32_t spi_clock_mhz = 1;

  ssp1__init(spi_clock_mhz);

  gpio__construct_with_function(GPIO__PORT_0, 7, GPIO__FUNCTION_2); // SCK1
  gpio__construct_with_function(GPIO__PORT_0, 8, GPIO__FUNCTION_2); // MISO1
  gpio__construct_with_function(GPIO__PORT_0, 9, GPIO__FUNCTION_2); // MOSI1

  gpio__set_function(SSD1306_cs_pin, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function(SSD1306_dcs_pin, GPIO__FUNCITON_0_IO_PIN);
  // gpio__set_function(vs1053b_dreq_pin, GPIO__FUNCITON_0_IO_PIN);
  // gpio__set_function(vs1053b_rst_pin, GPIO__FUNCITON_0_IO_PIN);

  gpio__set_as_output(SSD1306_cs_pin);
  gpio__set_as_output(SSD1306_dcs_pin);
  // gpio__set_as_output(vs1053b_rst_pin);

  // gpio__set_as_input(vs1053b_dreq_pin);

  gpio__set(SSD1306_cs_pin);
  gpio__set(SSD1306_dcs_pin);
  // gpio__set(vs1053b_rst_pin);
}

void SSD1306__cs() { gpio__reset(SSD1306_cs_pin); }
void SSD1306__ds() { gpio__set(SSD1306_cs_pin); }
void SSD1306__data_ds() { gpio__reset(SSD1306_dcs_pin); }
void SSD1306__data_cs() { gpio__set(SSD1306_dcs_pin); }

void SSD1306__transmit_byte(uint8_t byte) { (void)ssp1__exchange_byte(byte); }

#if 0
 ////////// left here //////////////////////
void vs1053b__reset_assert() { gpio__reset(vs1053b_rst_pin); }

void vs1053b__reset_deeassert() { gpio__set(vs1053b_rst_pin); }

void vs1053b__cs() { gpio__reset(vs1053b_cs_pin); }

void vs1053b__ds() { gpio__set(vs1053b_cs_pin); }

void vs1053b__dcs() { gpio__reset(vs1053b_dcs_pin); }

void vs1053b__dds() { gpio__set(vs1053b_dcs_pin); }

bool vs1053b__get_dreq() { return gpio__get(vs1053b_dreq_pin); }


uint8_t vs1053b__receive_byte() { return ssp0__exchange_byte(0xFF); }

void vs1053b__delay_ms(uint32_t ms) { delay__ms(ms); }

#endif