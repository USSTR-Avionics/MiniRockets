#include "BMI088.h"

/* accel object */
Bmi088Accel accel(Wire, 0x18);
/* gyro object */
Bmi088Gyro gyro(Wire, 0x68);

int init_bmi088()
	{
	int status;
	status = accel.begin();

	if (status < 0)
		{
		Serial.println("Accel Initialization Error");
		Serial.println(status);
		return EXIT_FAILURE;
		}

	status = gyro.begin();
	if (status < 0)
		{
		Serial.println("Gyro Initialization Error");
		Serial.println(status);
		return EXIT_FAILURE;
		}

	return EXIT_SUCCESS;
	}

float get_bmi088_gyro_x()
	{
	gyro.readSensor();
	return gyro.getGyroX_rads();
	}

float get_bmi088_gyro_y()
	{
	gyro.readSensor();
	return gyro.getGyroY_rads();
	}

float get_bmi088_gyro_z()
	{
	gyro.readSensor();
	return gyro.getGyroZ_rads();
	}