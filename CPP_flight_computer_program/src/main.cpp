#include "movingAverage.h"
/* NOT USING THESE RIGHT NOW; JUST TESTING OUT A MVP*/
//#include "kalmanFilter1dconst.h"
//#include "BasicLinearAlgebra.h"
//using namespace BLA;
//#include "Filter.h"
#include "sensor_ms5611.h"
#include "sensor_kx134.h"
#include <Arduino.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// PROGRAMMER VARS | vars for the programmer
bool debug = false; // remove these comparisons for production

// PROGRAM VARS | vars generally required for the program
unsigned long startingTime = 0;
#define MEM_ERR     0
#define INT_DEF    -1
#define FLO_DEF    -6.9

// SENSOR VARS | vars handling sensor data
float kx134_accel_x = FLO_DEF;
float kx134_accel_y = FLO_DEF;
float kx134_accel_z = FLO_DEF;
float ms5611_temp   = FLO_DEF;
float ms5611_press  = FLO_DEF;

// STATE VARS | vars that are important for the state machine
float absoluteAltitude = FLO_DEF;
int decentCheck        = INT_DEF;
float altitude         = 0.0;

// LIMIT VARS | vars defining important limits and thresholds
// NOTE: LIFTOFF_THRESHOLD could be 2 m/s^2, test to see what works best
#define LIFTOFF_THRESHOLD 9.9//1.15f (had 15)



// STATE MACHINE
struct rocketState 
    {
    bool groundIdle = false;
    bool poweredFlight = false;
    bool unpoweredFlight = false;
    bool ballisticDescent = false;
    bool chuteDescent = false;
    bool landSafe = false;
    } rocket;



void initAll() 
    {
    bool allValid = false;

    while (allValid == false)
        {
        init_kx134();
        init_MS5611();

        // !TODO clarify and condense the following block
        if (CrashReport) Serial.print(CrashReport);
            allValid = true;
            if (allValid == true) 
                {   
                rocket.groundIdle = true;
                }
        }
    }

void groundIdleMode(bool state)
    {
    if (state)
        {
        if (debug == true) 
            {
            Serial.println("[ROCKET STATE] GROUND IDLE");
            }

        kx134_accel_x = get_kx134_accel_x();
        kx134_accel_y = get_kx134_accel_y();
        kx134_accel_z = get_kx134_accel_z();
    
        if (abs(kx134_accel_z) > LIFTOFF_THRESHOLD)
            {
            // START TIMER: starting time is always 0 when running the code for the first time, if this is true set the starting time to the current time
            if (startingTime == 0UL)
                {
                startingTime = millis();
                }

            // DEBUG: Change from 2000 to 100
            // new time - starting time > 0.1 sec and accelation > threshold
            if ( (millis() - startingTime > 100) && (abs(kx134_accel_z) > LIFTOFF_THRESHOLD))
                {
                // reset the timer and go to next state
                startingTime = 0UL;
                rocket.poweredFlight = true;
                rocket.groundIdle = false;
                }
            // Otherwise restart the starting time since there was an issue
            else
                {
                startingTime = 0UL;
                }
            }
        }
    }


void poweredFlightMode(bool state)
    {
    if (state)
        {
        if (debug == true) 
            {
            Serial.println("[ROCKET STATE] POWERED FLIGHT");
            }

        if (abs(kx134_accel_z) < LIFTOFF_THRESHOLD)
            {
            // START TIMER: starting time is always 0 when running the code for the first time, if this is true set the starting time to the current time
            if (startingTime == 0UL)
                {
                startingTime = millis();
                }

            // new time - starting time > 0.1 sec and accelation > threshold
            //  DEBUG: Change from 5000 to 100
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

void apogeeCheck() 
    {
    //use pressure sensor to check for apogee
    // can also compare using timer
    // example: current_alt < (alt-1sec) -> Descending
    float last_alt = absoluteAltitude;

    if (decentCheck > 10) 
        {
        rocket.ballisticDescent = true;
        rocket.unpoweredFlight = false;
        }

    // GET BMP data on this line
    if (last_alt - absoluteAltitude > 2) 
        {
        decentCheck++;
        }
    }

void unpoweredFlightMode(bool state)
    {
    if (state)
        {
        if (debug == true) 
            {
            Serial.println("[ROCKET STATE] UNPOWERED FLIGHT");
            }
        apogeeCheck();
        }
    }

void ballisticDescentMode(bool state)
    {
    if (state)
        {
        if (debug == true) 
            {
            Serial.println("[ROCKET STATE] BALLISTIC DESCENT");
            }

        // 1000 ft = 304.8 m
        // Add a backup deployment height
        if (altitude < 304.8)
            {
            // START TIMER: starting time is always 0 when running the code for the first time
            if (startingTime == 0UL)
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
            if (startingTime == 0UL)
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
        }
    }

void debug_data()
    {
    delay(500);

    kx134_accel_x = get_kx134_accel_x();
    kx134_accel_y = get_kx134_accel_y();
    kx134_accel_z = get_kx134_accel_z();
    Serial.println("--- KX134 ---");
    Serial.print("x: ");
    Serial.println(kx134_accel_x);
    Serial.print("y: ");
    Serial.println(kx134_accel_y);
    Serial.print("z: ");
    Serial.println(kx134_accel_z);

    ms5611_temp = get_ms5611_temp();
    ms5611_press = get_ms5611_press();
    Serial.println("--- MS5611 ---");
    Serial.print("temperature: ");
    Serial.println(ms5611_temp);
    Serial.print("pressure: ");
    Serial.println(ms5611_press);
    }

// STANDARD ENTRY POINTS
void setup() 
    {
    Serial.begin(9600); // arg doesnt need to be 9600 just true
    Wire.begin();
    initAll();
    }

void loop() 
    {
    debug_data();

    groundIdleMode(rocket.groundIdle);
    poweredFlightMode(rocket.poweredFlight);
    unpoweredFlightMode(rocket.unpoweredFlight);
    ballisticDescentMode(rocket.ballisticDescent);
    chuteDescentMode(rocket.chuteDescent);
    landSafeMode(rocket.landSafe);
    }
