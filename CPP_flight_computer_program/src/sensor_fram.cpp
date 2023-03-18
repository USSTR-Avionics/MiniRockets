#include "Adafruit_FRAM_I2C.h"
#include <Arduino.h>
#include <stdint.h>

#define FRAM_I2C_ADDRESS 0x50

Adafruit_FRAM_I2C fram;


int init_fram()
	{
	fram = Adafruit_FRAM_I2C();
	fram.begin(FRAM_I2C_ADDRESS);

	return EXIT_SUCCESS;
	}

/*
 * @brief    direct lib API wrapper for writing to FRAM
 * @param    what    the data to write
 * @param    where   the address to write to
 */
int write_to_fram(uint8_t what, uint16_t where)
	{
	if (fram.write(where, what))
		{
		return EXIT_SUCCESS;
		}
	return EXIT_FAILURE;
	}

/*
 * @brief    direct lib API wrapper for reading from FRAM
 * @param    where   the address to read from
 * @return   the data read from FRAM
 */
uint8_t read_from_fram(int where)
	{
	return fram.read(where);
	}
