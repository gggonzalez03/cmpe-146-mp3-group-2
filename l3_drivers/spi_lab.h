/**********************************************************
 * CMPE 146 LAB : SPI Flash Interface
 *
 * This is a driver for SPI #2 on the LPC408x
 **********************************************************/
#pragma once
#include <stdint.h>
#include <stdio.h>

/**
 * Initializes the Serial Peripheral Interface (SPI)
 * @param max_clock_mhz is the desired SPI frequency in MHz
 **/
void spi__init(uint32_t max_clock_mhz);

/**
 * Send and receive data through SPI
 * @param data_out  is the output data from master to slave
 * @return          the received byte
 **/
uint8_t spi__exchange_byte(uint8_t data_out);