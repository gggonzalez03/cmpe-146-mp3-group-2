#pragma once

typedef enum {
  GPIO_INTR__FALLING_EDGE,
  GPIO_INTR__RISING_EDGE,
} gpio_interrupt_e;

typedef struct {
  uint8_t port;
  uint8_t pin;
  uint8_t edge;
} gpio_interrupt_s;

typedef void (*function_pointer_t)(void);

void gpiox__attach_interrupt(uint8_t port, uint8_t pin, gpio_interrupt_e interrupt_type, function_pointer_t callback);
void gpiox__interrupt_dispatcher();
void clear_pin_interrupt(uint8_t port, uint8_t pin);
gpio_interrupt_s gpiox__get_generated_interrupt();