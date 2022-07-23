/*
 * MS5611 Module
 * 
 * Product Link: https://www.smart-prototyping.com/image/cache/data/2020/10/102074%20MS5611/3-750x750.JPG
 * 
 * Inputs: VCC, GND, SCL, SDA, CSB, SDO, PS
 * 
 * Outputs:
 * - rawTemp
 * - rawPressure 
 * - realTemperature
 * - realPressure
 * - absoluteAltitude
 * - realAltitude
 * 
 * Set Up Instructions: 
 * 1. Connect VCC to 5V (accepts 3-5 V) and GND to GND
 * 2. Connect SCL to SCL (PIN 19 ON TEENSY) and SDA to SDA (PIN 18 ON TEENSY)
 *
 * Functions:
 * bool initMS5611(): initializes the sensor (0=error, 1=success)
 * getMS5611_Values(): updates rawTemp, rawPressure, realTemperature, realPressure, absoluteAltitude, relativeAltitude global variables
 * checkSettings(): an auxilary function used in initialization
 */

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

/*void setup() 
{
  
}
*/

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

/*void loop()
{
    Serial.print(" rawTemp = ");
  Serial.print(rawTemp);
  Serial.print(", realTemp = ");
  Serial.print(realTemperature);
  Serial.println(" *C");

  Serial.print(" rawPressure = ");
  Serial.print(rawPressure);
  Serial.print(", realPressure = ");
  Serial.print(realPressure);
  Serial.println(" Pa");

  Serial.print(" absoluteAltitude = ");
  Serial.print(absoluteAltitude);
  Serial.print(" m, relativeAltitude = ");
  Serial.print(relativeAltitude);    
  Serial.println(" m");

}*/
