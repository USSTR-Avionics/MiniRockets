#ifndef SENSOR_FRAM_H
#define SENSOR_FRAM_H

#include <stdint.h>

static int fram_cursor = 0x10;

#define FRAM_MAX_ADDRESS  0x7FE0
#define FRAM_MIN_ADDRESS  0x00
#define FRAM_INIT_ADDRESS 0x10

// actual funcs
int init_fram();

// ! These should not be used directly, use the package_fram.h API instead
int write_to_fram(uint16_t, uint8_t); 
int read_from_fram(int);              

#endif // SENSOR_FRAM_H
