#include "SSD1306_OLED_spi.h"
#include "delay.h"
#include "gpio.h"
#include "ssp1.h"

static gpio_s SSD1306_rst_pin = {GPIO__PORT_1, 23};

static gpio_s SSD1306_cs_pin = {GPIO__PORT_1, 14};

static gpio_s SSD1306_dcs_pin = {GPIO__PORT_4, 29};

void SSD1306__configure_spi() {

  const uint32_t spi_clock_mhz = 4;

  ssp1__init(spi_clock_mhz);

  gpio__construct_with_function(GPIO__PORT_0, 7, GPIO__FUNCTION_2); // SCK1
  gpio__construct_with_function(GPIO__PORT_0, 8, GPIO__FUNCTION_2); // MISO1
  gpio__construct_with_function(GPIO__PORT_0, 9, GPIO__FUNCTION_2); // MOSI1

  gpio__set_function(SSD1306_cs_pin, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function(SSD1306_dcs_pin, GPIO__FUNCITON_0_IO_PIN);

  gpio__set_as_output(SSD1306_rst_pin);
  gpio__set_as_output(SSD1306_cs_pin);
  gpio__set_as_output(SSD1306_dcs_pin);

  gpio__set(SSD1306_rst_pin);
  gpio__set(SSD1306_cs_pin);
  gpio__set(SSD1306_dcs_pin);
}

void SSD1306__rst_high() { gpio__set(SSD1306_rst_pin); }
void SSD1306__rst_low() { gpio__reset(SSD1306_rst_pin); }
void SSD1306__cs() { gpio__reset(SSD1306_cs_pin); }
void SSD1306__ds() { gpio__set(SSD1306_cs_pin); }
void SSD1306__data_ds() { gpio__reset(SSD1306_dcs_pin); } // write command
void SSD1306__data_cs() { gpio__set(SSD1306_dcs_pin); }   // write data

void SSD1306__transmit_byte(uint8_t byte) { (void)ssp1__exchange_byte(byte); }