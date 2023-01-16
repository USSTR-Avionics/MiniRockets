#ifndef MEMORY_FRAM_H
#define MEMORY_FRAM_H

#include "Adafruit_EEPROM_I2C.h"
#include "Adafruit_FRAM_I2C.h"


// debug funcs
int c_return_delay_test();

// actual funcs
int init_fram();
int write_temperature_to_fram(float);
int write_acceleration_to_fram(float);

#endif // MEMORY_FRAM_H