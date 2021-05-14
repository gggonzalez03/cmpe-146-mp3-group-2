#include "acceleration.h"
#include "i2c.h"

static const i2c_e acceleration__sensor_bus = I2C__2;
static const uint8_t acceleration__address = 0x38;
static const uint8_t acceleration__memory_axis_x = 0x01;
static const uint8_t acceleration__memory_axis_y = 0x03;
static const uint8_t acceleration__memory_axis_z = 0x05;
static const uint8_t acceleration__memory_control = 0x2A;
static const uint8_t acceleration__memory_who_am_i = 0x0D;

static void acceleration__activate(void) {
  uint8_t ctrl_reg_1 = 0x2A;
  uint8_t ctrl_reg_1_value;
  uint8_t activate = 0x01;

  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, ctrl_reg_1, &ctrl_reg_1_value, 1);
  i2c__write_single(acceleration__sensor_bus, acceleration__address, ctrl_reg_1, ctrl_reg_1_value | activate);
}

static void acceleration__deactivate(void) {
  uint8_t ctrl_reg_1 = 0x2A;
  uint8_t ctrl_reg_1_value;
  uint8_t deactivate = 0x01;

  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, ctrl_reg_1, &ctrl_reg_1_value, 1);
  i2c__write_single(acceleration__sensor_bus, acceleration__address, ctrl_reg_1, ctrl_reg_1_value & ~(deactivate));
}

static uint8_t acceleration__get_orientation_status_reg(void) {
  uint8_t orientation_reg = 0x10;
  uint8_t orientation_reg_value;

  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, orientation_reg, &orientation_reg_value, 1);

  return orientation_reg_value;
}

bool acceleration__init(void) {
  const uint8_t active_mode_with_100Hz = (1 << 0) | (3 << 3);
  i2c__write_single(acceleration__sensor_bus, acceleration__address, acceleration__memory_control,
                    active_mode_with_100Hz);

  const uint8_t who_am_i_expected_value = 0x2A;
  const uint8_t who_am_i_actual_value =
      i2c__read_single(acceleration__sensor_bus, acceleration__address, acceleration__memory_who_am_i);

  return (who_am_i_expected_value == who_am_i_actual_value);
}

float acceleration__get_acceleration_x(void) {
  uint8_t axis_x[2] = {0};

  int16_t raw_x = 0;
  float x = 0.0f;

  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, acceleration__memory_axis_x, axis_x,
                       sizeof(axis_x));

  raw_x = (((int16_t)axis_x[0] << 8) | axis_x[1]);
  x = (float)raw_x / 1024.0f / 16.0f * 9.8f;

  return x;
}

float acceleration__get_acceleration_y(void) {
  uint8_t axis_y[2] = {0};

  int16_t raw_y = 0;
  float y = 0.0f;

  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, acceleration__memory_axis_y, axis_y,
                       sizeof(axis_y));

  raw_y = (((int16_t)axis_y[0] << 8) | axis_y[1]);
  y = (float)raw_y / 1024.0f / 16.0f * 9.8f;

  return y;
}

float acceleration__get_acceleration_z(void) {
  uint8_t axis_z[2] = {0};

  int16_t raw_z = 0;
  float z = 0.0f;

  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, acceleration__memory_axis_z, axis_z,
                       sizeof(axis_z));

  raw_z = (((int16_t)axis_z[0] << 8) | (axis_z[1]));
  z = (float)raw_z / 1024.0f / 16.0f * 9.8f;

  return z;
}

void acceleration__enable_orientation_interrupts(void) {
  uint8_t ctrl_reg_4 = 0x2D;
  uint8_t orientation_cfg_reg = 0x11;
  uint8_t orientation_interrupt_en = 1 << 4;
  uint8_t orientation_detection_en = 1 << 6;

  acceleration__deactivate();

  i2c__write_single(acceleration__sensor_bus, acceleration__address, ctrl_reg_4, orientation_interrupt_en);
  i2c__write_single(acceleration__sensor_bus, acceleration__address, orientation_cfg_reg, orientation_detection_en);

  acceleration__activate();
}

acceleration__interrupt_source_e acceleration__get_interrupt_source(void) {
  uint8_t int_source_reg = 0x0C;
  uint8_t int_source;

  acceleration__deactivate();

  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, int_source_reg, &int_source, 1);

  acceleration__activate();

  return int_source;
}

acceleration__orientation_e acceleration__get_orientation(void) {
  uint8_t orientation_status_reg_val = acceleration__get_orientation_status_reg();

  uint8_t landscape_or_portrait = orientation_status_reg_val & (3UL << 2);
  uint8_t back_or_front = orientation_status_reg_val & (1UL << 0);

  acceleration__orientation_e orientation = landscape_or_portrait >> 1;

  if (back_or_front == 1) {
    return ACC__ORRIENTATION_UNKNOWN;
  }

  return orientation;
}