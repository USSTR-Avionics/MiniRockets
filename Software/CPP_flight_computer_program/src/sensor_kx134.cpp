#include "SparkFun_Qwiic_KX13X.h"

// As the rocket accelerates upwards
// the acceleration in the z direction becomes more nad more positive

QwiicKX134 kxAccel;
outputData accel_data;

#define g 9.8067f

/**
 * @brief  initialize the kx134 accelerometer
 * @return EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int init_kx134()
	{
	if (!kxAccel.begin())
		{
		return EXIT_FAILURE;
		}
	else if (!kxAccel.initialize(DEFAULT_SETTINGS))
		{ // Loading default settings.
		return EXIT_FAILURE;
		}
	else
		{
		// kxAccel.setRange(KX132_RANGE16G);
		// kxAccel.setRange(KX134_RANGE32G); // For a larger range uncomment
		return EXIT_SUCCESS;
		}
	}

/**
 * @brief  get the acceleration in the x direction
 * @return the acceleration in the x direction
 * @note   the acceleration is in g's
 */
float get_kx134_accel_x()
	{
	accel_data = kxAccel.getAccelData();
	return accel_data.xData * g; // g * 9.8 => raw accel in ms^-2
	}

/**
 * @brief  get the acceleration in the y direction
 * @return the acceleration in the y direction
 * @note   the acceleration is in g's
 */
float get_kx134_accel_y()
	{
	accel_data = kxAccel.getAccelData();
	return accel_data.yData * g;
	}

/**
 * @brief  get the acceleration in the z direction
 * @return the acceleration in the z direction
 * @note   the acceleration is in g's
 */
float get_kx134_accel_z()
	{
	accel_data = kxAccel.getAccelData();
	return accel_data.zData * g;
	}
