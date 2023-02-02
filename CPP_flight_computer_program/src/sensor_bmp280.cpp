#include "sensor_bmp280.h"
#include "Adafruit_BMP280.h"


Adafruit_BMP280 bmp; // use I2C interface
// Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor* bmp_pressure = bmp.getPressureSensor();


int init_bmp280()
	{
	if (!bmp.begin())
		{
		/* Default settings from datasheet. */
		bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
		                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
		                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
		                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
		                Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
		}

	return EXIT_SUCCESS;
	}

float get_bmp280_altitude(float ground_base_pressure_param)
	{
	return bmp.readAltitude(ground_base_pressure_param);
	}

float get_bmp280_pressure()
	{
	return bmp.readPressure();
	}

float get_bmp280_relative_altitude(float ground_base_pressure_param, float ground_base_altitude_param)
	{
	return get_bmp280_altitude(ground_base_pressure_param) - ground_base_altitude_param;
	}

float radians_to_degrees(float r)
	{
	return r * (float)(180 / M_PI);
	}

float degrees_to_radians(float d)
	{
	return d * (float)(M_PI / 180);
	}
