#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "uart_lab.h"

/**********************************************************
 *
 * 										PRIVATE VARIABLES
 *
 **********************************************************/
static QueueHandle_t rx_queue;

/**********************************************************
 *
 * 										PRIVATE FUNCTIONS
 *
 **********************************************************/

static void uart__lab_rx_3_interrupt_cb(void) {

  uint32_t interrupt_id = LPC_UART3->IIR & 0xF;
  uint32_t receive_data_available = 0x4;

  if (interrupt_id == receive_data_available) {
    char rx_byte = LPC_UART3->RBR;
    // fprintf(stderr, "Hi %d\n", rx_byte);
    xQueueSendFromISR(rx_queue, (const void *)&rx_byte, NULL);
  }
}

static void uart__lab_rx_2_interrupt_cb(void) {

  uint32_t interrupt_id = LPC_UART2->IIR & 0xF;
  uint32_t receive_data_available = 0x4;

  if (interrupt_id == receive_data_available) {
    char rx_byte = LPC_UART2->RBR;
    // fprintf(stderr, "Hi %d\n", rx_byte);
    xQueueSendFromISR(rx_queue, (const void *)&rx_byte, NULL);
  }
}

static void uart_lab__set_enable_interrupt_bit(uart_number_e uart) {
  LPC_UART_TypeDef *selected_uart = NULL;

  switch (uart) {
  case UART_2:
    selected_uart = LPC_UART2;
    break;
  case UART_3:
    selected_uart = LPC_UART3;
    break;
  default:
    break;
  }

  if (selected_uart == NULL)
    return;

  const uint32_t RBRIE = (1U << 0);
  selected_uart->IER |= RBRIE;
}

/**********************************************************
 *
 * 										PUBLIC FUNCTIONS
 *
 **********************************************************/

void uart_lab__init(uart_number_e uart, uint32_t peripheral_clock, uint32_t baud_rate) {

  LPC_UART_TypeDef *selected_uart = NULL;
  const uint32_t uart_2_power_on = (1 << 24);
  const uint32_t uart_3_power_on = (1 << 25);

  switch (uart) {
  case UART_2:
    LPC_SC->PCONP |= uart_2_power_on;
    selected_uart = LPC_UART2;
    break;
  case UART_3:
    LPC_SC->PCONP |= uart_3_power_on;
    selected_uart = LPC_UART3;
    break;
  default:
    break;
  }

  if (selected_uart == NULL)
    return;

  uart_lab__set_baud_rate(selected_uart, peripheral_clock, baud_rate);
}

void uart_lab__set_baud_rate(LPC_UART_TypeDef *uart, uint32_t peripheral_clock, uint32_t baud_rate) {

  uint32_t dlab_bit = (1U << 7);
  uint32_t word_length_8bits = (3U << 0);
  uint32_t divaddval = (0x0 << 0);
  uint32_t mulval = (0x1 << 4);

  // baud_rate = (clock_in_mhz) / 16 * (256*DLM + DLL) * (1 + DIVADD/DIVMUL)
  // baud_rate = (clock_in_mhz) / 16 * (256*DLM + DLL) * (1 + 0/1)
  // baud_rate = (clock_in_mhz) / 16 * ((DLM << 8) + DLL) * 1
  // baud_rate = (clock_in_mhz) / 16 * (divider_16_bit)
  // divider_16_bit = (clock_in_mhz) / (baud_rate << 4)
  const uint16_t divider_16_bit = (peripheral_clock * 1000 * 1000) / (baud_rate * 16);

  uart->LCR = word_length_8bits | dlab_bit;

  uart->DLL = (divider_16_bit >> 0) & 0xFF;
  uart->DLM = (divider_16_bit >> 8) & 0xFF;
  uart->FDR = divaddval | mulval; // basically zeroing out the fractional divider

  uart->LCR &= ~dlab_bit;
}

void uart_lab__configure_tx_rx_pins(gpio_s tx, gpio_s rx, gpio__function_e func) {
  gpio__construct_with_function(tx.port_number, tx.pin_number, func);
  gpio__construct_with_function(rx.port_number, rx.pin_number, func);
}

bool uart_lab__polled_get(uart_number_e uart, char *input_byte) {

  LPC_UART_TypeDef *selected_uart = NULL;

  switch (uart) {
  case UART_2:
    selected_uart = LPC_UART2;
    break;
  case UART_3:
    selected_uart = LPC_UART3;
    break;
  default:
    break;
  }

  if (selected_uart == NULL)
    return false;

  uint32_t rx_data_ready = (1 << 0);

  while (!(selected_uart->LSR & rx_data_ready)) {
    ;
  }

  *input_byte = selected_uart->RBR;

  return true;
}

bool uart_lab__polled_put(uart_number_e uart, char output_byte) {
  LPC_UART_TypeDef *selected_uart = NULL;

  switch (uart) {
  case UART_2:
    selected_uart = LPC_UART2;
    break;
  case UART_3:
    selected_uart = LPC_UART3;
    break;
  default:
    break;
  }

  if (selected_uart == NULL)
    return false;

  uint32_t tx_empty = (1 << 5);

  while (!(selected_uart->LSR & tx_empty)) {
    ;
  }

  selected_uart->THR = output_byte;

  return true;
}

void uart_lab__enable_receive_interrupt(uart_number_e uart_number) {

  lpc_peripheral_e selected_uart = 0;
  void (*uart__lab_rx_interrupt_cb)(void) = NULL;

  switch (uart_number) {
  case UART_2:
    selected_uart = LPC_PERIPHERAL__UART2;
    uart__lab_rx_interrupt_cb = uart__lab_rx_2_interrupt_cb;
    break;
  case UART_3:
    selected_uart = LPC_PERIPHERAL__UART3;
    uart__lab_rx_interrupt_cb = uart__lab_rx_3_interrupt_cb;
    break;

  default:
    break;
  }

  if (selected_uart == 0 || uart__lab_rx_interrupt_cb == NULL)
    return;

  uart_lab__set_enable_interrupt_bit(uart_number);
  lpc_peripheral__enable_interrupt(selected_uart, uart__lab_rx_interrupt_cb, "UART RXI");
  NVIC_EnableIRQ((IRQn_Type)selected_uart);

  rx_queue = xQueueCreate(20, sizeof(char));
}

bool uart_lab__get_char_from_queue(char *input_byte, uint32_t timeout) {
  return xQueueReceive(rx_queue, input_byte, timeout);
}