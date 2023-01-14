/*
 * BNO055 Module
 * 
 * Product Link: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-bno055-absolute-orientation-sensor.pdf
 * 
 * 
 * Inputs: VCC, GND, SCL, SDA, CSB, SDO, PS
 * 
 * Outputs:
 * - Acceleration (x,y,z) bno055_accel_x, bno055_accel_y, bno055_accel_z
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

/* Set the delay between fresh samples */
uint16_t BNO055_SAMPLERATE_DELAY_MS = 0;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.
   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.
   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3-5V DC
   Connect GROUND to common ground
   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

bool initBNO055() {
  //kSerial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    return false;
  }
  delay(1000);
  return true;
}

void get_bno055_data() {
  // put your main code here, to run repeatedly:
  //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  // VECTOR_EULER 1
  bno055_euler_x = bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();
  bno055_euler_y = bno.getVector(Adafruit_BNO055::VECTOR_EULER).y();
  bno055_euler_z = bno.getVector(Adafruit_BNO055::VECTOR_EULER).z();

  //2
  bno055_quat_w = bno.getQuat().w();
  bno055_quat_y = bno.getQuat().y();
  bno055_quat_x = bno.getQuat().x();
  bno055_quat_z = bno.getQuat().z();

  // VECTOR_GYROSCOPE 3
  bno055_ang_vel_x = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE).x();
  bno055_ang_vel_y = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE).y();
  bno055_ang_vel_z = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE).z();

  // VECTOR_ACCELEROMETER 4
  bno055_accel_x = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER).x();
  bno055_accel_y = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER).y();
  bno055_accel_z = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER).z();

  // VECTOR_MAGNETOMETER 5
  bno055_mag_x = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER).x();
  bno055_mag_y = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER).y();
  bno055_mag_z = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER).z();
  
  // VECTOR_LINEARACCEL 6
  bno055_linear_x = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL).x();
  bno055_linear_y = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL).y();
  bno055_linear_z = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL).z();

  // VECTOR_GRAVITY 7
  bno055_gravity_x = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY).x();
  bno055_gravity_y = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY).y();
  bno055_gravity_z = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY).z();

  // 8
  bno055_temp = bno.getTemp();
}
