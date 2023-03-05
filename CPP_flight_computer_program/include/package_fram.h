#ifndef PACKAGE_FRAM_H
#define PACKAGE_FRAM_H

#include <stdint.h>

#define WRITE_INTERVAL   500

int init_fram_package();
int write_data_chunk_to_fram(uint32_t, uint8_t, float, float, float, float, float, float, float, float, float);
int read_data_chunk_from_fram(uint32_t);
void dump_fram_to_serial();


typedef struct _sensor_chunk_layout
	{
	uint32_t timestamp;
	uint8_t current_state;
	float accl_x;
	float accl_y;
	float accl_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float rel_alt;
	float pressure;
	float thermocouple_temp;
	} sensor_chunk_layout;


// this is a global to store sensor_chunk data
// this is so that we can access the data from the main loop without a call to read_data_chunk_from_fram
extern sensor_chunk_layout sensor_chunk;

#endif // PACKAGE_FRAM_H