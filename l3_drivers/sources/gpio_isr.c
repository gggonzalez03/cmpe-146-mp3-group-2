#include <stdio.h>

#include "gpio_isr.h"
#include "lpc40xx.h"

// GPIO0 callback array for rising and falling edge interrupts
static function_pointer_t gpio0_callbacks[32][2];
// GPIO2 callback array for rising and falling edge interrupts
static function_pointer_t gpio2_callbacks[32][2];

/**
 * Attach a callback for an interrupt
 * @param port GPIO port number
 * @param pin pin number on the specified port
 * @param interrupt_type whether rising edge or falling edge
 * @param callback pointer to a function
 * @return void
 * */
void gpiox__attach_interrupt(uint8_t port, uint8_t pin, gpio_interrupt_e interrupt_type, function_pointer_t callback) {
  // 1) Store the callback based on the port and pin
  // 2) Configure GPIO 0 pin for rising or falling edge

  if (port == 0) {
    switch (interrupt_type) {
    case GPIO_INTR__RISING_EDGE:
      gpio0_callbacks[pin][GPIO_INTR__RISING_EDGE] = callback;
      LPC_GPIOINT->IO0IntEnR |= (1 << pin);
      break;
    case GPIO_INTR__FALLING_EDGE:
      gpio0_callbacks[pin][GPIO_INTR__FALLING_EDGE] = callback;
      LPC_GPIOINT->IO0IntEnF |= (1 << pin);
      break;

    default:
      break;
    }
  }

  else if (port == 2) {
    switch (interrupt_type) {
    case GPIO_INTR__RISING_EDGE:
      gpio2_callbacks[pin][GPIO_INTR__RISING_EDGE] = callback;
      LPC_GPIOINT->IO2IntEnR |= (1 << pin);
      break;
    case GPIO_INTR__FALLING_EDGE:
      gpio2_callbacks[pin][GPIO_INTR__FALLING_EDGE] = callback;
      LPC_GPIOINT->IO2IntEnF |= (1 << pin);
      break;

    default:
      break;
    }
  }
}

/**
 * Run the callback function depending on which pin caused the interrupt.
 * @return void
 * */
void gpiox__interrupt_dispatcher(void) {
  const gpio_interrupt_s gpio_interrupt = gpiox__get_generated_interrupt();
  function_pointer_t attached_user_handler = NULL;

  switch (gpio_interrupt.port) {
  case 0:
    attached_user_handler = gpio0_callbacks[gpio_interrupt.pin][gpio_interrupt.edge];
    break;
  case 2:
    attached_user_handler = gpio2_callbacks[gpio_interrupt.pin][gpio_interrupt.edge];
    break;

  default:
    break;
  }

  // Invoke the user registered callback, and then clear the interrupt
  if (attached_user_handler != NULL) {
    attached_user_handler();
  }
  clear_pin_interrupt(gpio_interrupt.port, gpio_interrupt.pin);
}

/**
 * Get info about the generated interrupt
 * @return interrupt_details containing port, pin, and edge information
 * */
gpio_interrupt_s gpiox__get_generated_interrupt() {
  gpio_interrupt_s interrupt_details = {0xFF, 0xFF, 0xFF};

  uint32_t pin_position = 0x00000000;

  if (LPC_GPIOINT->IO0IntStatR) {
    pin_position = LPC_GPIOINT->IO0IntStatR;
    interrupt_details.port = 0;
    interrupt_details.edge = GPIO_INTR__RISING_EDGE;
  } else if (LPC_GPIOINT->IO0IntStatF) {
    pin_position = LPC_GPIOINT->IO0IntStatF;
    interrupt_details.port = 0;
    interrupt_details.edge = GPIO_INTR__FALLING_EDGE;
  } else if (LPC_GPIOINT->IO2IntStatR) {
    pin_position = LPC_GPIOINT->IO2IntStatR;
    interrupt_details.port = 2;
    interrupt_details.edge = GPIO_INTR__RISING_EDGE;
  } else if (LPC_GPIOINT->IO2IntStatF) {
    pin_position = LPC_GPIOINT->IO2IntStatF;
    interrupt_details.port = 2;
    interrupt_details.edge = GPIO_INTR__FALLING_EDGE;
  }

  while (pin_position) {
    pin_position = pin_position >> 1;
    interrupt_details.pin++;
  }

  return interrupt_details;
}

/**
 * Clear GPIOx Interrupt
 * @param port GPIO port number
 * @param pin pin number on the specified port
 * @return void
 * */
void clear_pin_interrupt(uint8_t port, uint8_t pin) {
  switch (port) {
  case 0:
    LPC_GPIOINT->IO0IntClr |= (1 << pin);
    break;
  case 2:
    LPC_GPIOINT->IO2IntClr |= (1 << pin);
    break;

  default:
    break;
  }
}
