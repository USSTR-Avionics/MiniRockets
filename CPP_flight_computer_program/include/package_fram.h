#ifndef PACKAGE_FRAM_H
#define PACKAGE_FRAM_H

#include <stdint.h>

int init_fram_package();
int write_data_chunk_to_fram(uint32_t, uint8_t, float, float, float, float, float, float, float, float, float, uint16_t);
void read_data_chunk_from_fram(uint8_t);

#endif // PACKAGE_FRAM_H