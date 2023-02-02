#include "sensor_fram.h"
#include <Arduino.h>
#include <stdint.h>

#define ERR_READING  0
#define WRITE_OFFSET 0x10
int fram_cursor = 0x10;

void dump_fram_to_serial()
	{
	// TODO: build csv string and dump to USB serial
	}

int write_temperature_to_fram(float temp)
	{
	// TODO: test and work
	float upper_bound = 100.0f;
	float lower_bound = -100.0f;

	if (temp > upper_bound || temp < lower_bound)
		{
		write_to_fram(fram_cursor, ERR_READING);
		fram_cursor += 1;
		return EXIT_FAILURE;
		}

	uint8_t whole_part   = (uint8_t)temp;
	uint8_t decimal_part = (uint8_t)((temp - whole_part) * 1000);
	whole_part += 101;

	write_to_fram(fram_cursor, whole_part);
	fram_cursor += 1;
	write_to_fram(fram_cursor, decimal_part);
	fram_cursor += 1;

	return EXIT_SUCCESS;
	}

float read_temperature_from_fram(int where)
	{
	// TODO:
	int whole = read_from_fram(where);
	where++; // read the next byte
	int decimal       = read_from_fram(where);

	float temperature = ((float)whole - 101.0f) + ((float)decimal / 100.0f);

	return temperature;
	}

/**
 * @brief Writes the accelerometer data to the FRAM.
 * @param accel The accelerometer data to write to the FRAM.
 * @return EXIT_SUCCESS if the write succeeded, EXIT_FAILURE otherwise.
 */
int write_acceleration_to_fram(float accel)
	{
	// TODO: test and work
	// handle negative accelation values
	uint8_t whole_part   = (uint8_t)accel;
	uint8_t decimal_part = (uint8_t)((accel - whole_part) * 1000);
	whole_part += 101;

	write_to_fram(fram_cursor, whole_part);
	fram_cursor += 1;
	write_to_fram(fram_cursor, decimal_part);
	fram_cursor += 1;

	return EXIT_SUCCESS;
	}

float read_acceleration_from_fram(int where)
	{
	// TODO:
	where = 0;
	return 0.0f;
	}
