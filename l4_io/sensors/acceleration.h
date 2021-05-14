#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  ACC__SRC_DRDY = 0,
  ACC__SRC_FF_MT = 4,
  ACC__SRC_PULSE = 8,
  ACC__SRC_LNDPRT = 16,
  ACC__SRC_TRANS = 32,
  ACC__SRC_ASLP = 64
} acceleration__interrupt_source_e;

typedef enum {
  ACC__ORRIENTATION_UP = 0,
  ACC__ORRIENTATION_DOWN,
  ACC__ORRIENTATION_RIGHT,
  ACC__ORRIENTATION_LEFT,
  ACC__ORRIENTATION_UNKNOWN
} acceleration__orientation_e;

bool acceleration__init(void);

float acceleration__get_acceleration_x(void);

float acceleration__get_acceleration_y(void);

float acceleration__get_acceleration_z(void);

void acceleration__enable_orientation_interrupts(void);

acceleration__orientation_e acceleration__get_orientation(void);

acceleration__interrupt_source_e acceleration__get_interrupt_source(void);