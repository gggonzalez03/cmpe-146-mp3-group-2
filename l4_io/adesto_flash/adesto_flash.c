#include "adesto_flash.h"
#include "gpio.h"
#include "spi_lab.h"

static const gpio_s adesto_cs_pin = {1, 10};
static const uint8_t adesto_dummy_byte = 0xFF;

/**********************************************************
 *
 * 										PRIVATE FUNCTIONS
 *
 **********************************************************/

static void adesto_spi_transmit_address(uint32_t address) {
  uint8_t byte_mask = 0xFF;
  (void)spi__exchange_byte(address >> 16 & byte_mask);
  (void)spi__exchange_byte(address >> 8 & byte_mask);
  (void)spi__exchange_byte(address >> 0 & byte_mask);
}

/**********************************************************
 *
 * 										PUBLIC FUNCTIONS
 *
 **********************************************************/

/**
 * De-assert CS pin to start SPI transmission
 **/
void adesto_cs(void) { gpio__reset(adesto_cs_pin); }

/**
 * Assert CS pin to end SPI transmission
 **/
void adesto_ds(void) { gpio__set(adesto_cs_pin); }

/**
 * Read Adesto Flash device signature
 * @return adesto_flash_id_s object that contains the relevant information
 **/
adesto_flash_id_s adesto_read_signature(void) {
  adesto_flash_id_s data = {0};
  const uint8_t op_code = 0x9F;

  adesto_cs();
  {
    /**
     * My board's external flash is AT25SF041 (not AT25DN256)
     * The manufacturing id is the same, but device id is actually different
     * First byte of id: 0x84
     * Second byte of id: 0x01
     * There's also no extended device id for this chip
     **/
    (void)spi__exchange_byte(op_code);
    data.manufacturer_id = spi__exchange_byte(adesto_dummy_byte);
    data.device_id_1 = spi__exchange_byte(adesto_dummy_byte);
    data.device_id_2 = spi__exchange_byte(adesto_dummy_byte);
    // data.extended_device_id = spi__exchange_byte(adesto_dummy_byte);
  }
  adesto_ds();

  return data;
}

void adesto_write_enable(void) {
  uint8_t we_op_code = 0x06;

  adesto_cs();
  (void)spi__exchange_byte(we_op_code);
  adesto_ds();
}

void adesto_write_disable(void) {
  uint8_t we_op_code = 0x04;

  adesto_cs();
  (void)spi__exchange_byte(we_op_code);
  adesto_ds();
}

/**
 * Erase a block of memory on the Adesto Flash
 * @param address contains the 24-bit starting address of the block to erase
 * @param size    is the size of block to erase
 **/
void adesto_erase_block(uint32_t address, adesto_block_e size) {
  uint8_t block_erase_op_code = 0xFF;

  switch (size) {
  case BLOCK_4KB:
    block_erase_op_code = 0x20;
    break;
  case BLOCK_32KB:
    block_erase_op_code = 0x52;
    break;
  case BLOCK_64KB:
    block_erase_op_code = 0xD8;
    break;
  default:
    break;
  }

  /**
   * TODO: Check the flash memory's busy status first
   * */
  if (block_erase_op_code != 0xFF) {
    adesto_cs();
    {
      (void)spi__exchange_byte(block_erase_op_code);
      adesto_spi_transmit_address(address);
    }
    adesto_ds();
  }
}

/**
 * Write to the Adesto Flash. Sizes greater than 256 will loop back in the same page.
 * @param address constains the 24-bit starting address of space to write to
 * @param data    pointer to the array of data to be written
 * @param size    is the size of the data array in bytes to write
 **/
void adesto_write(uint32_t address, uint8_t *data, uint16_t size) {
  static const uint8_t byte_or_page_program_op_code = 0x02;

  /**
   * TODO: Check the flash memory's busy status first
   * */

  adesto_cs();
  {
    (void)spi__exchange_byte(byte_or_page_program_op_code);
    adesto_spi_transmit_address(address);
    for (uint16_t i = 0; i < size; i++) {
      (void)spi__exchange_byte(data[i]);
    }
  }
  adesto_ds();
}

/**
 * Read from the Adesto Flash
 * @param address constains the 24-bit starting address of space to read from
 * @param data    pointer to the data buffer
 * @param size    is the size of the data array in bytes to read
 **/
void adesto_read(uint32_t address, uint8_t *data, uint16_t size) {
  // static const uint8_t read_array_50MHz_max_op_code = 0x03;
  static const uint8_t read_array_85MHz_max_op_code = 0x0B;

  /**
   * TODO: Check the flash memory's busy status first
   * */
  adesto_cs();
  {
    (void)spi__exchange_byte(read_array_85MHz_max_op_code);
    adesto_spi_transmit_address(address);
    // Extra dummy byte required (See Section 6.1 of the datasheet)
    (void)spi__exchange_byte(adesto_dummy_byte);
    for (uint16_t i = 0; i < size; i++) {
      data[i] = spi__exchange_byte(adesto_dummy_byte);
    }
  }
  adesto_ds();
}

/**
 * Checks if the Adesto Flash is busy
 * adesto_cs() must be called first and adesto_ds() must be called after
 * calling this function. This function can be polled before calling adesto_cd()
 * @param started whether op_code has been sent or not. This should be 1 when polling.
 * @return busy status, 0: not busy, 1: busy
 **/
uint8_t adesto_is_busy(uint8_t started) {
  static const uint8_t read_status_register_op_code = 0x05;

  if (!started) {
    (void)spi__exchange_byte(read_status_register_op_code);
  }

  if (spi__exchange_byte(adesto_dummy_byte) & 0x01) {
    return 1;
  }
  return 0;
}

/**
 * Wait until Adesto Flash is not busy
 **/
void adesto_wait_until_not_busy() {
  adesto_cs();
  adesto_is_busy(0);
  while (adesto_is_busy(1)) {
    ;
  }
  adesto_ds();
}