#include "Adafruit_MAX31855.h"


#define MAXDO  3
#define MAXCS  4
#define MAXCLK 5

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

int init_thermocouple()
	{
	if (thermocouple.begin())
		{
		return EXIT_SUCCESS;
		}
	else
		{
		return EXIT_FAILURE;
		}
	}

float read_thermocouple_celsius()
	{
	return thermocouple.readCelsius();
	}