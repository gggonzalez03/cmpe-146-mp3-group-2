/**********************************************************
 * CMPE 146 LAB : UART
 *
 * This is a driver for UART on the LPC408x
 **********************************************************/
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"
#include "lpc40xx.h"

typedef enum { UART_2 = 2, UART_3 } uart_number_e;

/**
 * Initialize a UART peripheeral
 * @param uart              which UART to initialize
 * @param peripheral_clock  is the frequency of the clock in MHz being fed to the peripheral
 * @param baud_rate         desired baud_rate in bps
 **/
void uart_lab__init(uart_number_e uart, uint32_t peripheral_clock, uint32_t baud_rate);

/**
 * Set the baud rate by setting the DLL and DLM bytes
 * @param uart              pointer to the UART peripheral to set baud rate of
 * @param peripheral_clock  is the frequency of the clock in MHz being fed to the peripheral
 * @param baud_rate         desired baud_rate in bps
 **/
void uart_lab__set_baud_rate(LPC_UART_TypeDef *uart, uint32_t peripheral_clock, uint32_t baud_rate);

/**
 * Configure pins as UART_TX and UART_RX
 * @param tx    transmit pin
 * @param rx    receive pin
 * @param func  FUNC bits for both the pins. This function assumes that tx and rx
 *              pins are configured with the same func bits
 **/
void uart_lab__configure_tx_rx_pins(gpio_s tx, gpio_s rx, gpio__function_e func);

/**
 * Polled byte read from Receiver Buffer Register (RBR)
 * @param uart        which UART to read from
 * @param input_byte  address where to save the read value
 * @return            read successful or not
 **/
bool uart_lab__polled_get(uart_number_e uart, char *input_byte);

/**
 * Polled byte write to Transmit Holding Register (THR)
 * @param uart        which UART to write to
 * @param output_byte byte to write to THR
 * @return            write successful or not
 **/
bool uart_lab__polled_put(uart_number_e uart, char output_byte);

/**
 * Enable receive interrupt for a selected UART
 * @param uart_numbeer  is the selected uart to enable receive interrupt for
 **/
void uart_lab__enable_receive_interrupt(uart_number_e uart_number);

/**
 * Get a char from the UART receive queue
 * @param input_byte  char variable to store the 8 bit data
 * @param timeout     amount of time willing to wait
 **/
bool uart_lab__get_char_from_queue(char *input_byte, uint32_t timeout);