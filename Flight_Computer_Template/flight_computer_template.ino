/*
 * USST Avionics Flight Computer
 * Iteration 1
 * 
 * Modules Included:
 * 
 */

//-------------PRE-PROCESSOR VARIABLES-----------
//* NOTE: LIFTOFF_THRESHOLD could be 2 m/s^2, test to see what works best
#define LIFTOFF_THRESHOLD 1.15f


//-------------LIBRARIES AND MODULES-------------
//#include "Tester.h"


//-------------OBJECT DECLARATION----------------
//Tester Test1(5);

//-------------VARIABLES-------------------------

float initial_altitude = 0.0;
int decentCheck;

// For Testing, Remove later
float pressure = 0.0;
float ax, ay, az;
float timer = 0.0;
float altitude = 0.0;
int value = 0;
unsigned long startingTime = 0;




/*
 * STATE MACHINE:
 * 1. Ground Idle: Rocket has not launched
 * 2. Powered Flight: Rocket has launched
 * 3. Unpowered Flight: Burnout has occured (engines are no longer powering rocket)
 * 4. Ballistic Descent: Apogee has been reached
 * 5. Chute Descent: Pyros have fired and parachute is deployed
 * 6. Land Safe: Rocket is back on the ground
 * 
 * For all our functions to access
 * Access states by typing rocket.yourStateHere
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
  
  //----GPS_NEO6M----
  //Initialize

  //Check Value
  bool gps_valid = true;
  
  //----IMU----
  //Initialize

  //Check Value

  //----BME280----
  //Initialize

  //Check Value

  //----Temperature----
  //Initialize

  //Check Value

  //----MicroSD----
  //Initialize

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

    ledON("GREEN");
    //buzzerOn();
    
    // GET ACCELERATION FROM IMU
    if (abs(az) > LIFTOFF_THRESHOLD)
    {
      // START TIMER: starting time is always 0 when running the code for the first time
      if (startingTime = 0UL)
      {
        startingTime = millis();
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      if ( (millis() - startingTime > 100) && (abs(az) > LIFTOFF_THRESHOLD))
      {
        // reset the timer and go to next state
        startingTime = 0;
        rocket.poweredFlight = true;
        rocket.groundIdle = false;
      } 
    }
  }

}

void poweredFlightMode(bool state)
{

  if (state)
  {
    if (abs(az) < LIFTOFF_THRESHOLD)
    {
      // START TIMER: starting time is always 0 when running the code for the first time
      if (startingTime = 0UL)
      {
        startingTime = millis();
      }
      // new time - starting time > 0.1 sec and accelation > threshold
      if ( (millis() - startingTime > 100) && (abs(az) < LIFTOFF_THRESHOLD))
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
    apogeeCheck();
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
  Serial.print(pressure);
  Serial.print(",");
  Serial.print(altitude);
  delay(500);
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("setup");
  initAll();
}

void loop() {
  groundIdleMode(rocket.groundIdle);
  poweredFlightMode(rocket.poweredFlight);
  unpoweredFlightMode(rocket.unpoweredFlight);
  ballisticDescentMode(rocket.ballisticDescent);
  chuteDescentMode(rocket.chuteDescent);
  landSafeMode(rocket.landSafe);
  dataReadout();
  
  
  // This is just here to test integration with C++
  /*
  Serial.println("Test C++");
  value = Test1.testCPlusPlus(4);
  Serial.println(value);
  */
}
