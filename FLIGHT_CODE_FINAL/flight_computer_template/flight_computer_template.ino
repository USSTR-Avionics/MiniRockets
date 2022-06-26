/*
   USST Avionics Flight Computer
   Iteration 2

   Modules Included:

*/



//-------------PRE-PROCESSOR VARIABLES-----------
//* NOTE: LIFTOFF_THRESHOLD could be 2 m/s^2, test to see what works best
#define LIFTOFF_THRESHOLD 9.9//1.15f (had 15)


//-------------LIBRARIES AND MODULES-------------
//#include "Tester.h"
#include <Wire.h>
//#include <SD.h>
//#include <SPI.h>
#include "SparkFun_Qwiic_KX13X.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <MS5611.h>

//-------------OBJECT DECLARATION----------------
//Tester Test1(5);

//-------------VARIABLES-------------------------

float initial_altitude = 0.0;
int decentCheck;

float timer = 0.0;
float sensor1 = 0;
float altitude = 0.0;

unsigned long startingTime = 0;
float x =1;


//QwiicKX132 kxAccel;
QwiicKX134 kxAccel; // Uncomment this if using the KX134 - check your board
//if unsure.
outputData kx134_accel; // This will hold the accelerometer's output.
float kx134_accel_x;
float kx134_accel_y;
float kx134_accel_z;
float bno055_accel_x;
float bno055_accel_y;
float bno055_accel_z;
float bno055_orient_x;
float bno055_orient_y;
float bno055_orient_z;
float bno055_gyro_x;
float bno055_gyro_y;
float bno055_gyro_z;
float bno055_linear_x;
float bno055_linear_y;
float bno055_linear_z;
float bno055_mag_x;
float bno055_mag_y;
float bno055_mag_z;
float bno055_gravity_x;
float bno055_gravity_y;
float bno055_gravity_z;
float bno055_temp;
float bno055_calib_sys;
float bno055_calib_gyro;
float bno055_calib_accel;
float bno055_calib_mag;


MS5611 ms5611;
uint32_t rawTemp;
uint32_t rawPressure;
double realTemperature;
long realPressure;
float absoluteAltitude;
float relativeAltitude;


double referencePressure;

/*
   STATE MACHINE:
   1. Ground Idle: Rocket has not launched
   2. Powered Flight: Rocket has launched
   3. Unpowered Flight: Burnout has occured (engines are no longer powering rocket)
   4. Ballistic Descent: Apogee has been reached
   5. Chute Descent: Pyros have fired and parachute is deployed
   6. Land Safe: Rocket is back on the ground

   For all our functions to access
   Access states by typing rocket.yourStateHere
*/
struct rocketState {
  bool groundIdle = false;
  bool poweredFlight = false;
  bool unpoweredFlight = false;
  bool ballisticDescent = false;
  bool chuteDescent = false;
  bool landSafe = false;
} rocket;


// All the sensors are initialized and checked for proper functionality
void initAll() {
  bool allValid = false;
  while (allValid == false)
  {

    //----KX134_ACCEL----
    initKX134();

    //----GPS_NEO6M----
    //Initialize

    //Check Value
    bool gps_valid = true;

    //----IMU----
    //Initialize
    initBNO055();
    //Check Value

    //----BME280----
    //Initialize
    initMS5611();

    //Check Value

    //----Temperature----
    //Initialize

    //Check Value

    //----MicroSD----
    //Initialize
    //initMicroSD();

    //Check Value

    //----FlashChip----
    //Initialize

    //Check Value

    //----LED----
    //Initialize
    LED_initSensor();

    //----Buzzer----

    //----LoRa Module Placeholder----
    //Initialize

    //Check Value

    //----Parachute Deployment Placeholder----
    //Initialize

    //Check Value
    allValid = gps_valid;
    if (allValid == true) {
      rocket.groundIdle = true;
    }
  }
}


void groundIdleMode(bool state)
{

  if (state)
  {
    // Debug Start

    Serial.println("GROUND IDLE");
    // Debug End

    ledON("GREEN");
    //buzzerOn();
    sensor1 = 20;

    // GET ACCELERATION FROM IMU
    getKX134_Accel();
    get_bno055_data();
    //getAccel();
    //getGyro();
    //Serial.print(startingTime);
    //Serial.print(millis());
    if (abs(kx134_accel_z) > LIFTOFF_THRESHOLD)
    {
      //Serial.println("IT IS GREATER");
      // START TIMER: starting time is always 0 when running the code for the first time
      if (startingTime == 0UL)
      {
        //Serial.print("YEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
        startingTime = millis();
      }
      // Changed from 2000 to 500
      // new time - starting time > 0.1 sec and accelation > threshold
      if ( (millis() - startingTime > 100) && (abs(kx134_accel_z) > LIFTOFF_THRESHOLD))
      {
        // reset the timer and go to next state
        startingTime = 0UL;
        rocket.poweredFlight = true;
        rocket.groundIdle = false;
      }
      else
      {
        startingTime == 0UL;
      }


    }
  }

}

void poweredFlightMode(bool state)
{

  if (state)
  {
    dataReadout();
    // Debug Start
    //Serial.println("POWERED FLIGHT");
    // Debug End
    ledON("BLUE");
    getKX134_Accel();
    //getGyro();
    if (abs(kx134_accel_z) < LIFTOFF_THRESHOLD)
    {
      //Serial.print("LESS THAN");
      // START TIMER: starting time is always 0 when running the code for the first time
      if (startingTime = 0UL)
      {
        startingTime = millis();
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      // Changed from 5000 to 100
      if ( (millis() - startingTime > 100) && (abs(kx134_accel_z) < LIFTOFF_THRESHOLD))
      {
        // reset the timer and go to next state
        startingTime = 0;
        rocket.unpoweredFlight = true;
        rocket.poweredFlight = false;
      }

    }
  }

}

//use pressure sensor to check for apogee
// can also compare using timer
// example: current_alt < (alt-1sec) -> Descending
void apogeeCheck() {
  getMS5611_Values();
  float last_alt = altitude;
  if (decentCheck > 10) {
    rocket.ballisticDescent = true;
    rocket.unpoweredFlight = false;
  }
  // GET BMP data on this line
  if (last_alt - altitude > 2) {
    decentCheck++;
  }
}

void unpoweredFlightMode(bool state)
{
  if (state)
  {
    
    dataReadout();
    Serial.println("UNPOWERED FLIGHT");
    ledON("RED");
    // GET ACCELERATION FROM IMU
    //getKX134_Accel();

    //getGyro();
    //apogeeCheck();
  }
}

void ballisticDescentMode(bool state)
{
  if (state)
  {

    // 1000 ft = 304.8 m
    // Add a backup deployment height
    if (altitude < 304.8)
    {
      // START TIMER: starting time is always 0 when running the code for the first time
      if (startingTime = 0UL)
      {
        startingTime = millis();
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      if ( (millis() - startingTime > 100) && (altitude < 304.8))
      {
        // reset the timer and go to next state
        startingTime = 0;
        rocket.chuteDescent = true;
        rocket.ballisticDescent = false;
      }

    }
  }

}

void chuteDescentMode(bool state)
{

  if (state)
  {

    dataReadout();
    // DEPLOY PARACHUTE
    if (altitude < 5)
    {

      // START TIMER
      // START TIMER: starting time is always 0 when running the code for the first time
      if (startingTime = 0UL)
      {
        startingTime = millis();
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      if ( (millis() - startingTime > 100) && (altitude < 5))
      {
        // reset the timer and go to next state
        startingTime = 0;
        rocket.landSafe = true;
        rocket.chuteDescent = false;
      }
    }

  }
}

void landSafeMode(bool state)
{
  if (state)
  {
    // STOP DATA COLLECTION
    // CHECK IF SD CARD CAN STILL BE WRITTEN TO
    // IF SD CARD CAN BE WRITTEN TO AND FLASHCHIP OK
    // WRITE TO SD CARD
    buzzerOn();
    ledON("GREEN");

  }
}

void dataReadout() {
  getKX134_Accel();
  get_bno055_data();
  getMS5611_Values();
  Serial.print(x);
  Serial.print(",");
  Serial.print(kx134_accel_x);
  Serial.print(",");
  Serial.print(kx134_accel_y);
  Serial.print(",");
  Serial.print(kx134_accel_z);
  Serial.print(",");
  Serial.print(bno055_accel_x);
  Serial.print(",");
  Serial.print(bno055_accel_y);
  Serial.print(",");
  Serial.print(bno055_accel_z);
  Serial.print(",");
  Serial.print(bno055_orient_x);
  Serial.print(",");
  Serial.print(bno055_orient_y);
  Serial.print(",");
  Serial.print(bno055_orient_z);
  Serial.print(",");
  Serial.print(bno055_gyro_x);
  Serial.print(",");
  Serial.print(bno055_gyro_y);
  Serial.print(",");
  Serial.print(bno055_gyro_z);
  Serial.print(",");
  Serial.print(bno055_linear_x);
  Serial.print(",");
  Serial.print(bno055_linear_y);
  Serial.print(",");
  Serial.print(bno055_linear_z);
  Serial.print(",");
  Serial.print(bno055_mag_x);
  Serial.print(",");
  Serial.print(bno055_mag_y);
  Serial.print(",");
  Serial.print(bno055_mag_z);
  Serial.print(",");
  Serial.print(bno055_gravity_x);
  Serial.print(",");
  Serial.print(bno055_gravity_y);
  Serial.print(",");
  Serial.print(bno055_gravity_z);
  Serial.print(",");
  Serial.print(bno055_temp);
  Serial.print(",");
  Serial.print(bno055_calib_sys);
  Serial.print(",");
  Serial.print(bno055_calib_gyro);
  Serial.print(",");
  Serial.print(bno055_calib_accel);
  Serial.print(",");
  Serial.print(bno055_calib_mag);
  Serial.print(",");
  Serial.print(rawTemp);
  Serial.print(",");
  Serial.print(rawPressure);
  Serial.print(",");
  Serial.print(realTemperature);
  Serial.print(",");
  Serial.print(realPressure);
  Serial.print(",");
  Serial.print(absoluteAltitude);
  Serial.print(",");
  Serial.println(relativeAltitude);
  delay(1);
  //writeToMicroSD();
  x=x+1;
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Wire.begin();
  //Serial.println("setup");
  initAll();
}

void loop() {
      //dataReadout();
  groundIdleMode(rocket.groundIdle);
  poweredFlightMode(rocket.poweredFlight);
  unpoweredFlightMode(rocket.unpoweredFlight);
  ballisticDescentMode(rocket.ballisticDescent);
  chuteDescentMode(rocket.chuteDescent);
  landSafeMode(rocket.landSafe);

}
