#include "sensor_thermocouple.h"
#include "Adafruit_MAX31855.h"
#include "sensor_pins.h"
#include <SPI.h>

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

/**
 * @brief    initialize the thermocouple
 * @note     this is a void function as initialization is not needed
 */
void init_thermocouple()
	{
	return;
	}

/**
 * @note    reads temperature at the end of the wire
 * @return  float external temperature in degrees Celsius
 */
float get_thermocouple_external_temperature()
	{
	float external_temp = thermocouple.readCelsius();
	return external_temp;
	}

/**
 * @note    reads temperature on the chip
 * @return  float internal temperature in degrees Celsius
 */
float get_thermocouple_internal_temperature()
	{
	float internal_temp = thermocouple.readInternal();
	return internal_temp;
	}