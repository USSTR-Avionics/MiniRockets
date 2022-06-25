/*
  MS5611 Barometric Pressure & Temperature Sensor. Simple Example
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/czujnik-cisnienia-i-temperatury-ms5611.html
  GIT: https://github.com/jarzebski/Arduino-MS5611
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
*/


#include <MS5611.h>

MS5611 ms5611;
uint32_t rawTemp;
uint32_t rawPressure;
double realTemperature;
long realPressure;
float absoluteAltitude;
float relativeAltitude;


double referencePressure;

bool initMS5611() {
  // Initialize MS5611 sensor
  Serial.println("Initialize MS5611 Sensor");

  while(!ms5611.begin())
  {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    return 0;
  }

  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();

  // Check settings
  checkSettings();
  return 1;
}

//void setup() 
//{
//  
//
//  
//}

void checkSettings()
{
  Serial.print("Oversampling: ");
  Serial.println(ms5611.getOversampling());
}

void getMS5611_Values() {
  // Read raw values
  rawTemp = ms5611.readRawTemperature();
  rawPressure = ms5611.readRawPressure();

  // Read true temperature & Pressure
  realTemperature = ms5611.readTemperature();
  realPressure = ms5611.readPressure();

  // Calculate altitude
  absoluteAltitude = ms5611.getAltitude(realPressure);
  relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);
}

//void loop()
//{
//    Serial.print(" rawTemp = ");
//  Serial.print(rawTemp);
//  Serial.print(", realTemp = ");
//  Serial.print(realTemperature);
//  Serial.println(" *C");
//
//  Serial.print(" rawPressure = ");
//  Serial.print(rawPressure);
//  Serial.print(", realPressure = ");
//  Serial.print(realPressure);
//  Serial.println(" Pa");
//
//  Serial.print(" absoluteAltitude = ");
//  Serial.print(absoluteAltitude);
//  Serial.print(" m, relativeAltitude = ");
//  Serial.print(relativeAltitude);    
//  Serial.println(" m");
//
//}
