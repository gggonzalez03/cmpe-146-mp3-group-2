#pragma once

#include <stdbool.h>
#include <stdint.h>

bool accleration_i2c_recognize(void);

uint16_t get_acceleration_x(void);

uint16_t get_acceleration_y(void);

uint16_t get_acceleration_z(void);
