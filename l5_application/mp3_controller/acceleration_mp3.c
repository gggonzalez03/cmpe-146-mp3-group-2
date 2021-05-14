#include "acceleration_mp3.h"
#include "i2c.h"

static const i2c_e acceleration__sensor_bus = I2C__2;
static const uint8_t acceleration__address = 0x38;
static const uint8_t acceleration__memory_axis_x = 0x01;
static const uint8_t acceleration__memory_axis_y = 0x03;
static const uint8_t acceleration__memory_axis_z = 0x04;
static const uint8_t acceleration__memory_control = 0x2A;
static const uint8_t acceleration__memory_who_am_i = 0x0D;

bool accleration_i2c_recognize(void) {
  const uint8_t active_mode_with_100Hz = (1 << 0) | (3 << 3);
  i2c__write_single(acceleration__sensor_bus, acceleration__address, acceleration__memory_control,
                    active_mode_with_100Hz);

  const uint8_t who_am_i_expected_value = 0x2A;
  const uint8_t who_am_i_actual_value =
      i2c__read_single(acceleration__sensor_bus, acceleration__address, acceleration__memory_who_am_i);

  return (who_am_i_expected_value == who_am_i_actual_value);
}

uint16_t get_acceleration_x(void) {
  uint8_t axis_x[2] = {0};
  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, acceleration__memory_axis_x, axis_x,
                       sizeof(axis_x));
  return (((uint16_t)axis_x[0] << 8) | axis_x[1]);
}

uint16_t get_acceleration_y(void) {
  uint8_t axis_y[2] = {0};
  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, acceleration__memory_axis_y, axis_y,
                       sizeof(axis_y));
  return (((uint16_t)axis_y[0] << 8) | axis_y[1]);
}

uint16_t get_acceleration_z(void) {
  uint8_t axis_z[2] = {0};
  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, acceleration__memory_axis_z, axis_z,
                       sizeof(axis_z));
  return (((uint16_t)axis_z[0] << 8) | axis_z[1]);
}
