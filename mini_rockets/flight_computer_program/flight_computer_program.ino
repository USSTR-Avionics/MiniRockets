/*
   USST Avionics Mini Rocket Flight Computer
   Iteration 1

   Modules Included:
   - LED
   - BUZZER
   - BNO055 IMU
   - MS5611 PRESSURE SENSOR
   - MICROSD 
   - TX RFM95
   - Kalman Filter
   - 

*/



//-------------PRE-PROCESSOR VARIABLES-----------
//* NOTE: LIFTOFF_THRESHOLD could be 2 m/s^2, test to see what works best



//-------------LIBRARIES AND MODULES-------------
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <MS5611.h>
#include "externs.h"
#include "SparkFun_Qwiic_KX13X.h"
#include "Adafruit_FRAM_I2C.h"
#include "movingAverage.h"
//#include "kalmanFilter1dconst.h"
#include <BasicLinearAlgebra.h>
using namespace BLA;
#include "Filter.h"

//-------------OBJECT DECLARATION----------------
MS5611 ms5611;
QwiicKX134 kxAccel;

//-------------VARIABLES-------------------------
// KX134
outputData kx134_accel; // This will hold the accelerometer's output.

float initial_altitude = 0.0;
int decentCheck;
float timer = 0.0;
float transmit_timer = 0.0;
float altitude = 0.0;
unsigned long startingTime = 0;
float x =1;

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
  if (mode == 0)
{
  ledActivate = 1;
  buzzerActivate = 1;
  MS5611Activate = 1;
  microSdActivate = 0;
  KX134Activate = 0;
  FRAMActivate = 0;
  TXActivate = 0;
  kalmanFilterActivate = 0;
  movingAverageFilterActivate = 0;
  #define LIFTOFF_THRESHOLD 5.0//1.15f (had 15)
  #define APOGEE_THRESHOLD 0.1
  #define PARACHUTE_DEPLOYMENT_HEIGHT 0.1
  #define LAND_SAFE_HEIGHT 0.0
  #define TIMER_COUNT_THRESHOLD 50.0
}
if (mode == 1)
{
  #define LIFTOFF_THRESHOLD 8.0//1.15f (had 15)
  #define APOGEE_THRESHOLD 0.5
  #define PARACHUTE_DEPLOYMENT_HEIGHT 800
  #define LAND_SAFE_HEIGHT 700.0
  #define TIMER_COUNT_THRESHOLD 100.0
}
if (mode == 2)
{
  #define LIFTOFF_THRESHOLD 8.0//1.15f (had 15)
  #define APOGEE_THRESHOLD 0.5
  #define PARACHUTE_DEPLOYMENT_HEIGHT 800
  #define LAND_SAFE_HEIGHT 700.0
  #define TIMER_COUNT_THRESHOLD 10.0
}
    initKX134();
    init_FRAM();
    initBNO055();
    initMS5611();
    initMicroSD();
    LED_initSensor();
    init_RFM95_TX();
    rocket.groundIdle = true;
    
}


void groundIdleMode(bool state)
{
  if (state)
  {
    currentState = 1;
    ledON("GREEN");
    //buzzerOn();
    get_bno055_data();
    
    if (abs(bno055_linear_z) > LIFTOFF_THRESHOLD)
    {
      // START TIMER: starting time is always 0 when running the code for the first time, if this is true set the starting time to the current time
      if (startingTime == 0UL)
      {
        startingTime = millis();
      }
      // DEBUG: Change from 2000 to 100
      // new time - starting time > 0.1 sec and accelation > threshold
      if ( (millis() - startingTime > TIMER_COUNT_THRESHOLD) && (abs(bno055_linear_z) > LIFTOFF_THRESHOLD))
      {
        // reset the timer and go to next state
        startingTime = 0UL;
        rocket.poweredFlight = true;
        rocket.groundIdle = false;
      }
      // Otherwise restart the starting time since there was an issue
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
    currentState = 2;
    ledON("BLUE");

    
    if (abs(bno055_linear_z) < LIFTOFF_THRESHOLD)
    {
      // START TIMER: starting time is always 0 when running the code for the first time, if this is true set the starting time to the current time
      if (startingTime = 0UL)
      {
        startingTime = millis();
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      //  DEBUG: Change from 5000 to 100
      if ( (millis() - startingTime > TIMER_COUNT_THRESHOLD) && (abs(bno055_linear_z) < LIFTOFF_THRESHOLD))
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
   // START TIMER: starting time is always 0 when running the code for the first time, if this is true set the starting time to the current time
      if (startingTime == 0UL)
      {
        startingTime = millis();
        last_alt = relativeAltitude;
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      //  DEBUG: Change from 5000 to 100
      if ( (millis() - startingTime > TIMER_COUNT_THRESHOLD) && (last_alt - relativeAltitude > APOGEE_THRESHOLD))
      {
        // reset the timer and increase decentCheck;
        startingTime = 0;
        decentCheck++;
      }
  if (decentCheck > 7) {
    rocket.ballisticDescent = true;
    rocket.unpoweredFlight = false;
  }
  getMS5611_Values();
  // was 2 m
  if (last_alt - relativeAltitude > APOGEE_THRESHOLD) {
    decentCheck++;
  }
}

void unpoweredFlightMode(bool state)
{
  if (state)
  {
    currentState = 3;
 
    ledON("RED");
    apogeeCheck();
    
  }
}

void ballisticDescentMode(bool state)
{
  if (state)
  {

      currentState = 4;

    // 1000 ft = 304.8 m
    // Add a backup deployment height
    if (relativeAltitude < PARACHUTE_DEPLOYMENT_HEIGHT)
    {
      // START TIMER: starting time is always 0 when running the code for the first time
      if (startingTime = 0UL)
      {
        startingTime = millis();
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      if ( (millis() - startingTime > TIMER_COUNT_THRESHOLD) && (relativeAltitude < PARACHUTE_DEPLOYMENT_HEIGHT))
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
    currentState = 5;
    // DEPLOY PARACHUTE
    if (relativeAltitude < LAND_SAFE_HEIGHT)
    {

      // START TIMER
      // START TIMER: starting time is always 0 when running the code for the first time
      if (startingTime = 0UL)
      {
        startingTime = millis();
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      if ( (millis() - startingTime > TIMER_COUNT_THRESHOLD) && (relativeAltitude < LAND_SAFE_HEIGHT))
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
    currentState = 5;
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
  
  serialPrint();
      write_fram();
      writeToMicroSD();

      // START TIMER: starting time is always 0 when running the code for the first time, if this is true set the starting time to the current time
      if (transmit_timer == 0UL)
      {
        transmit_timer = millis();
      }
      
      // new time - starting time > 5 sec and accelation > threshold
      if ( millis() - transmit_timer > 0 ) //5000=5 sec
      {
        // reset the timer and go to next state
        transmit_timer = 0UL;
        sendPacket();
      }
      
  if (kalmanFilterActivate == 1)
  {
      iterate(absoluteAltitude,kx134_accel_z,startkalman);
      startkalman = startkalman+1;
      //kalmanAverage = kalmanFilter(absoluteAltitude);
      //kalmanGain = get_k();
  }
  if (movingAverageFilterActivate == 1)
  {
      updateNumCount();
  }
  x=x+1;
}

void updateNumCount()
{
  
    if (numCounter==0)
    {
        val0 = absoluteAltitude;
    }
    else if (numCounter==1)
    {
        val1 = absoluteAltitude;
    }
    else if (numCounter==2)
    {
        val2 = absoluteAltitude;
    }
    else
    {
        val3 = absoluteAltitude;
    }

    if (numCounter<3)
    {
        numCounter+=1;
    }
    else
    {
        numCounter=0;
    }
    movingAverageVal = movingAverage(absoluteAltitude,x,val0,val1,val2,val3);
    
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Wire.begin();
  initAll();
}

void loop() {
  dataReadout();
  groundIdleMode(rocket.groundIdle);
  poweredFlightMode(rocket.poweredFlight);
  unpoweredFlightMode(rocket.unpoweredFlight);
  ballisticDescentMode(rocket.ballisticDescent);
  chuteDescentMode(rocket.chuteDescent);
  landSafeMode(rocket.landSafe);
}
