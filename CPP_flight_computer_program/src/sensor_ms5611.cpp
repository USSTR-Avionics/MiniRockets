#include <MS5611.h>
 
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

    while(!ms5611.begin())
    {
      Serial.println("Could not find a valid MS5611 sensor, check wiring!");
      return 0;
    }

    // Check settings
    checkSettings();

    return 1;
    }

double get_ms5611_temp()
    {
    double temp_data = ms5611.getTemperature();
    return temp_data;
    }

double get_ms5611_press()
    {
    double press_data = ms5611.getPressure();
    Serial.println("before");
    Serial.println(press_data);
    return press_data;
    }