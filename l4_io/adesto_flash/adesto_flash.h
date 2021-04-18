/**********************************************************
 * CMPE 146 LAB : SPI Flash Interface
 *
 * This is a library for using the Adesto Flash Memory (AT25SF041)
 * through SPI2 of the LPC4078 mcu from NXP
 **********************************************************/
#pragma once

#include <stdint.h>

typedef struct {
  uint8_t manufacturer_id;
  uint8_t device_id_1;
  uint8_t device_id_2;
  uint8_t extended_device_id;
} adesto_flash_id_s;

typedef enum { BLOCK_4KB, BLOCK_32KB, BLOCK_64KB } adesto_block_e;

/**
 * De-assert CS pin to start SPI transmission
 **/
void adesto_cs(void);

/**
 * Assert CS pin to end SPI transmission
 **/
void adesto_ds(void);

/**
 * Read Adesto Flash device signature
 * @return adesto_flash_id_s object that contains the relevant information
 **/
adesto_flash_id_s adesto_read_signature(void);

/**
 * Enable writes to the adesto flash memory
 **/
void adesto_write_enable(void);

/**
 * Disable writes to the adesto flash memory
 **/
void adesto_write_disable(void);

/**
 * Erase a block of memory on the Adesto Flash
 * @param address contains the 24-bit starting address of the block to erase
 * @param size    is the size of block to erase
 **/
void adesto_erase_block(uint32_t address, adesto_block_e size);

/**
 * Write to the Adesto Flash
 * @param address constains the 24-bit starting address of space to write to
 * @param data    pointer to the array of data to be written
 * @param size    is the size of the data array in bytes to write
 **/
void adesto_write(uint32_t address, uint8_t *data, uint16_t size);

/**
 * Read from the Adesto Flash
 * @param address constains the 24-bit starting address of space to read from
 * @param data    pointer to the data buffer
 * @param size    is the size of the data array in bytes to read
 **/
void adesto_read(uint32_t address, uint8_t *data, uint16_t size);

/**
 * Checks if the Adesto Flash is busy
 * adesto_cs() must be called first and adesto_ds() must be called after
 * calling this function. This function can be polled before calling adesto_cd()
 * @param started whether op_code has been sent or not. This should be 1 when polling.
 * @return busy status, 0: not busy, 1: busy
 **/
uint8_t adesto_is_busy(uint8_t started);

/**
 * Wait until Adesto Flash is not busy
 **/
void adesto_wait_until_not_busy();