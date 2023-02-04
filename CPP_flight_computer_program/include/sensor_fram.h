#ifndef SENSOR_FRAM_H
#define SENSOR_FRAM_H

#include <stdint.h>

#define FRAM_MAX_ADDRESS     0x7FE0
#define FRAM_MIN_ADDRESS     0x00
#define FRAM_INIT_ADDRESS    0x10

// actual funcs
int init_fram();
int write_to_fram(uint16_t what, uint8_t where); // this should not be exposed
int read_from_fram(int);     // this should not be exposed

#endif // SENSOR_FRAM_H
