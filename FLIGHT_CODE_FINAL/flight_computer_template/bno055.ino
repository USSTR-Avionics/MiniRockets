/*
 * BNO055 Module
 * 
 * Product Link: https://www.smart-prototyping.com/image/cache/data/2020/10/102074%20MS5611/3-750x750.JPG
 * 
 * Inputs: VCC, GND, SCL, SDA, CSB, SDO, PS
 * 
 * Outputs:
 * - Acceleration (x,y,z) bno055_accel_x, bno055_accel_y, bno055_accel_z
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
    while (1);
  }

  delay(1000);
}

/*void getBNO055_Accel() {
  bno055_accel_x = event->acceleration.x;
  bno055_accel_y = event->acceleration.y;
  bno055_accel_z = event->acceleration.z;
}*/

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

  printEvent(&orientationData);
  printEvent(&angVelocityData);
  printEvent(&linearAccelData);
  printEvent(&magnetometerData);
  printEvent(&accelerometerData);
  printEvent(&gravityData);

  int8_t boardTemp = bno.getTemp();
  //kSerial.println();
  //kSerial.print(F("temperature: "));
  //kSerial.println(boardTemp);

  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  //kSerial.println();
  ///kSerial.print("Calibration: Sys=");
  //kSerial.print(system);
  //kSerial.print(" Gyro=");
  //kSerial.print(gyro);
  //kSerial.print(" Accel=");
  //kSerial.print(accel);
  //kSerial.print(" Mag=");
  //kSerial.println(mag);

  //kSerial.println("--");
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

//void setup(void)
//{
//  Serial.begin(115200);
//  Serial.println("Orientation Sensor Test"); Serial.println("");
//
//  /* Initialise the sensor */
//  if (!bno.begin())
//  {
//    /* There was a problem detecting the BNO055 ... check your connections */
//    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
//    while (1);
//  }
//
//  delay(1000);
//}

//void loop(void)
//{
//
//}



void printEvent(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    //kSerial.print("Accl:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    bno055_accel_x = event->acceleration.x;
    bno055_accel_y = event->acceleration.y;
    bno055_accel_z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    //kSerial.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
    bno055_orient_x = event->orientation.x;
    bno055_orient_y = event->orientation.y;
    bno055_orient_z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    //kSerial.print("Mag:");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
    bno055_mag_x = event->magnetic.x;
    bno055_mag_y = event->magnetic.y;
    bno055_mag_z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    //kSerial.print("Gyro:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
    bno055_gyro_x = event->gyro.x;
    bno055_gyro_y = event->gyro.y;
    bno055_gyro_z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    //kSerial.print("Rot:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
    bno055_gyro_x = event->gyro.x;
    bno055_gyro_y = event->gyro.y;
    bno055_gyro_z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    //kSerial.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
    bno055_linear_x = event->acceleration.x;
    bno055_linear_y = event->acceleration.y;
    bno055_linear_z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    //kSerial.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
  // k Serial.print("Unk:");
  }

  //kSerial.print("\tx= ");
  //kSerial.print(x);
  //kSerial.print(" |\ty= ");
  //kSerial.print(y);
  //kSerial.print(" |\tz= ");
  //kSerial.println(z);
}
