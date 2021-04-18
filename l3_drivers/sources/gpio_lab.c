#include "gpio_lab.h"

#include "lpc40xx.h"

/// Should alter the hardware registers to set the pin as input
void gpiox__set_as_input(uint8_t port_num, uint8_t pin_num) {
  LPC_GPIO_TypeDef *port = NULL;
  switch (port_num) {
  case 0:
    port = LPC_GPIO0;
    break;
  case 1:
    port = LPC_GPIO1;
    break;
  case 2:
    port = LPC_GPIO2;
    break;

  default:
    break;
  }

  if (port != NULL) {
    port->DIR &= ~(1 << pin_num);
  }
}

/// Should alter the hardware registers to set the pin as output
void gpiox__set_as_output(uint8_t port_num, uint8_t pin_num) {
  LPC_GPIO_TypeDef *port = NULL;
  switch (port_num) {
  case 0:
    port = LPC_GPIO0;
    break;
  case 1:
    port = LPC_GPIO1;
    break;
  case 2:
    port = LPC_GPIO2;
    break;

  default:
    break;
  }

  if (port != NULL) {
    port->DIR |= (1 << pin_num);
  }
}

/// Should alter the hardware registers to set the pin as high
void gpiox__set_high(uint8_t port_num, uint8_t pin_num) {
  LPC_GPIO_TypeDef *port = NULL;
  switch (port_num) {
  case 0:
    port = LPC_GPIO0;
    break;
  case 1:
    port = LPC_GPIO1;
    break;
  case 2:
    port = LPC_GPIO2;
    break;

  default:
    break;
  }

  if (port != NULL) {
    port->PIN |= (1 << pin_num);
  }
}

/// Should alter the hardware registers to set the pin as low
void gpiox__set_low(uint8_t port_num, uint8_t pin_num) {
  LPC_GPIO_TypeDef *port = NULL;
  switch (port_num) {
  case 0:
    port = LPC_GPIO0;
    break;
  case 1:
    port = LPC_GPIO1;
    break;
  case 2:
    port = LPC_GPIO2;
    break;

  default:
    break;
  }

  if (port != NULL) {
    port->PIN &= ~(1 << pin_num);
  }
}

/**
 * Should alter the hardware registers to set the pin as low
 *
 * @param {bool} high - true => set pin high, false => set pin low
 */
void gpiox__set(uint8_t port_num, uint8_t pin_num, bool high) {

  LPC_GPIO_TypeDef *port = NULL;
  switch (port_num) {
  case 0:
    port = LPC_GPIO0;
    break;
  case 1:
    port = LPC_GPIO1;
    break;
  case 2:
    port = LPC_GPIO2;
    break;

  default:
    break;
  }

  if (high) {
    port->SET = (1 << pin_num);
  } else {
    port->CLR = (1 << pin_num);
  }
}

/**
 * Should return the state of the pin (input or output, doesn't matter)
 *
 * @return {bool} level of pin high => true, low => false
 */
bool gpiox__get_level(uint8_t port_num, uint8_t pin_num) {

  LPC_GPIO_TypeDef *port = NULL;
  switch (port_num) {
  case 0:
    port = LPC_GPIO0;
    break;
  case 1:
    port = LPC_GPIO1;
    break;
  case 2:
    port = LPC_GPIO2;
    break;

  default:
    break;
  }

  if (port == NULL) {
    return false;
  }

  if (port->PIN & (1 << pin_num)) {
    return true;
  } else {
    return false;
  }
}