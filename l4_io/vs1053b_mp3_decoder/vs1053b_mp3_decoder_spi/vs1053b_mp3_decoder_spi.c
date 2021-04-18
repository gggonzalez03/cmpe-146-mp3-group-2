#include "vs1053b_mp3_decoder_spi.h"
#include "delay.h"
#include "gpio.h"
#include "ssp0.h"

static gpio_s vs1053b_cs_pin = {GPIO__PORT_2, 8};

static gpio_s vs1053b_dcs_pin = {GPIO__PORT_2, 7};

static gpio_s vs1053b_rst_pin = {GPIO__PORT_2, 6};

static gpio_s vs1053b_dreq_pin = {GPIO__PORT_0, 16};

void vs1053b__configure_spi() {

  const uint32_t spi_clock_mhz = 1;

  ssp0__init(spi_clock_mhz);

  gpio__construct_with_function(GPIO__PORT_0, 15, GPIO__FUNCTION_2); // SCK0
  gpio__construct_with_function(GPIO__PORT_0, 17, GPIO__FUNCTION_2); // MISO0
  gpio__construct_with_function(GPIO__PORT_0, 18, GPIO__FUNCTION_2); // MOSI0

  gpio__set_function(vs1053b_cs_pin, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function(vs1053b_dcs_pin, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function(vs1053b_dreq_pin, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function(vs1053b_rst_pin, GPIO__FUNCITON_0_IO_PIN);

  gpio__set_as_output(vs1053b_cs_pin);
  gpio__set_as_output(vs1053b_dcs_pin);
  gpio__set_as_output(vs1053b_rst_pin);

  gpio__set_as_input(vs1053b_dreq_pin);

  gpio__set(vs1053b_cs_pin);
  gpio__set(vs1053b_dcs_pin);
  gpio__set(vs1053b_rst_pin);
}

void vs1053b__reset_assert() { gpio__reset(vs1053b_rst_pin); }

void vs1053b__reset_deeassert() { gpio__set(vs1053b_rst_pin); }

void vs1053b__cs() { gpio__reset(vs1053b_cs_pin); }

void vs1053b__ds() { gpio__set(vs1053b_cs_pin); }

void vs1053b__dcs() { gpio__reset(vs1053b_dcs_pin); }

void vs1053b__dds() { gpio__set(vs1053b_dcs_pin); }

bool vs1053b__get_dreq() { return gpio__get(vs1053b_dreq_pin); }

void vs1053b__transmit_byte(uint8_t byte) { (void)ssp0__exchange_byte(byte); }

uint8_t vs1053b__receive_byte() { return ssp0__exchange_byte(0xFF); }

void vs1053b__delay_ms(uint32_t ms) { delay__ms(ms); }