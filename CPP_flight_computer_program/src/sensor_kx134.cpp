#include "SparkFun_Qwiic_KX13X.h"

QwiicKX134 kxAccel;
outputData accel_data;

#define g 9.8067f

bool init_kx134()
	{
	if (!kxAccel.begin())
		{
		return 0;
		}
	else if (!kxAccel.initialize(DEFAULT_SETTINGS))
		{ // Loading default settings.
		return 0;
		}
	else
		{
		// kxAccel.setRange(KX132_RANGE16G);
		// kxAccel.setRange(KX134_RANGE32G); // For a larger range uncomment
		return 1;
		}
	}

float get_kx134_accel_x()
	{
	accel_data = kxAccel.getAccelData();
	return accel_data.xData * g; // g * 9.8 => raw accel in ms^-2
	}

float get_kx134_accel_y()
	{
	accel_data = kxAccel.getAccelData();
	return accel_data.yData * g;
	}

float get_kx134_accel_z()
	{
	// TODO: calibrate the sensor with angle to get the cos and sin component of the vector
	accel_data = kxAccel.getAccelData();
	return accel_data.zData * g * -1; // becuase accelereation up towards the sky is negative in the z direction
	}

bool confirm_acceleration_z()
	{
	uint8_t count                      = 0;
	uint8_t deceleration_count         = 5;
	float last_acceleration_reading    = 1000;
	float current_acceleration_reading = 0.0f;
	while (count < 5)
		{
		current_acceleration_reading = get_kx134_accel_z();
		if (current_acceleration_reading < last_acceleration_reading)
			{
			deceleration_count += 1;
			}
		count += 1;
		}
	return true;
	}
