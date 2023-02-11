#include <MS5611.h>
#include <math.h>



MS5611 ms5611;

void checkSettings()
	{
	Serial.print("Oversampling: ");
	Serial.println(ms5611.getOversampling());
	}

bool init_MS5611()
	{
	// Initialize MS5611 sensor
	Serial.println("Initialize MS5611 Sensor");

	while (!ms5611.begin())
		{
		Serial.println("Could not find a valid MS5611 sensor, check wiring!");
		return 0;
		}


	ms5611.setOversampling(OSR_ULTRA_HIGH);

	// Check settings
	// checkSettings();

	return 1;
	}

float get_ms5611_temp()
	{
	int result = ms5611.read();
	if (result != MS5611_READ_OK)
		{
		Serial.println("[MS5611] Error in read");
		Serial.println(result);
		}
	float temp_data = ms5611.getTemperature();
	return temp_data;
	}

float get_ms5611_press()
	{
	int result = ms5611.read();
	if (result != MS5611_READ_OK)
		{
		//! Shouldn't this return an invalid reading?
		// Serial.println("[MS5611] Error in read");
		// Serial.println(result);
		}
	float press_data = ms5611.getPressure();
	return press_data;
	}

float get_ms5611_altitude(float pressure, float seaLevelPressure)
	{
	return (44330.0f * (1.0f - (float)pow(pressure / seaLevelPressure, 0.1902949f)));
	}