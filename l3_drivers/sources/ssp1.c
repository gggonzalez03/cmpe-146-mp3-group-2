/**********************************************************
 * CMPE 146 LAB : SPI Flash Interface
 *
 * This is a driver for SPI #2 on the LPC408x
 **********************************************************/
#include "ssp1.h"
#include "clock.h"
#include "lpc40xx.h"

/**********************************************************
 *
 * 										PRIVATE FUNCTIONS
 *
 **********************************************************/
static void ssp1__power_on() {
  uint32_t spi_1_power_on = (1 << 19);
  LPC_SC->PCONP |= spi_1_power_on;
}

static void ssp1__setup_clock(uint32_t max_clock_mhz) {
  uint32_t CPSDVSR_mask = 0xFF;
  uint32_t spi_clock = clock__get_peripheral_clock_hz();

  for (uint32_t divider = 2; divider < 255; divider += 2) {
    if ((spi_clock / divider) <= (max_clock_mhz * 1000UL * 1000UL)) {
      LPC_SSP1->CPSR &= ~(CPSDVSR_mask);
      LPC_SSP1->CPSR |= divider;
      break;
    }
  }
}

static void ssp1__setup_cr0() {
  uint32_t data_size_select_8_bits = (0b111);
  uint32_t frame_format_spi = (0b00 << 4);

  LPC_SSP1->CR0 = data_size_select_8_bits | frame_format_spi;
}

static void ssp1__setup_cr1() {
  uint32_t spi_enable = 0b1 << 1;

  LPC_SSP1->CR1 |= spi_enable;
}

static void ssp1__setup_control_registers() {
  ssp1__setup_cr0();
  ssp1__setup_cr1();
}

/**********************************************************
 *
 * 										PUBLIC FUNCTIONS
 *
 **********************************************************/

/**
 * Initializes the Serial Peripheral Interface (SPI)
 * @param max_clock_mhz is the desired SPI frequency in MHz
 **/
void ssp1__init(uint32_t max_clock_mhz) {
  ssp1__power_on();
  ssp1__setup_clock(max_clock_mhz);
  ssp1__setup_control_registers();
}

/**
 * Send and receive data through SPI
 * @param data_out  is the output data from master to slave
 * @return          the received byte
 **/
uint8_t ssp1__exchange_byte(uint8_t data_out) {
  // Write to DATA register
  LPC_SSP1->DR = (uint32_t)data_out;

  // Wait until the transmit is done
  while (LPC_SSP1->SR & (1 << 4)) {
    ;
  }

  // Then read the DATA register again
  return (uint8_t)LPC_SSP1->DR & 0xFF;
}