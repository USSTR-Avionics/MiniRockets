#include "Adafruit_FRAM_I2C.h"
#include <Arduino.h>
#include <stdint.h>

#define FRAM_I2C_ADDRESS 0x50

Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();


int init_fram()
	{
	Serial.println("init_fram()");
	fram.begin(FRAM_I2C_ADDRESS);

	return EXIT_SUCCESS;
	}

/*
 * @brief    direct lib API wrapper for writing to FRAM
 * @param    what    the data to write
 * @param    where   the address to write to
 */
int write_to_fram(int what, int where)
	{
	fram.write(where, what);
	return EXIT_SUCCESS;
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