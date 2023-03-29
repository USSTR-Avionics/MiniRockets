#include "sensor_bmp280.h"
#include "Adafruit_BMP280.h"


Adafruit_BMP280 bmp; // use I2C interface
// Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor* bmp_pressure = bmp.getPressureSensor();


/**
 * @brief     initialize the BMP280
 * @return    EXIT_SUCCESS or EXIT_FAILURE
*/
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

/**
 * @brief     get the altitude from the BMP280
 * @param     ground_base_pressure_param   the pressure at ground level to be used as a reference
 * @brief     this should not be used to get altitude actively, use get_bmp280_relative_altitude() instead
 * @return    the altitude in meters
*/
float get_bmp280_altitude(float ground_base_pressure_param)
	{
	return bmp.readAltitude(ground_base_pressure_param);
	}

/**
 * @brief    get the pressure from the BMP280
 * @return   the pressure in Pascals
*/
float get_bmp280_pressure()
	{
	return bmp.readPressure();
	}

/**
 * @brief    get the relative altitude from the BMP280
 * @param    ground_base_pressure_param   the pressure at ground level to be used as a reference
 * @param    ground_base_altitude_param   the altitude at ground level to be used as a reference
 * @return   the altitude in meters 
*/
float get_bmp280_relative_altitude(float ground_base_pressure_param, float ground_base_altitude_param)
	{
	return get_bmp280_altitude(ground_base_pressure_param) - ground_base_altitude_param;
	}

