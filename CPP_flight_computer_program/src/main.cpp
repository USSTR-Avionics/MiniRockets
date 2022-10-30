/* NOT USING THESE RIGHT NOW; JUST TESTING OUT A MVP*/
//#include "kalmanFilter1dconst.h"
//#include "BasicLinearAlgebra.h"
//#include "movingAverage.h"
//using namespace BLA;
//#include "Filter.h"
#include "default_variables.h"
#include "sensor_ms5611.h"
#include "sensor_sdcard.h"
#include "sensor_kx134.h"
#include "memory_fram.h"
#include "errorcodes.h"
#include "rusty_fram.h"
#include "watchdog.h"
#include <Arduino.h>
#include <RH_RF95.h>
#include <stdint.h> // switch to machine independent types
#include <stdlib.h>
#include <Wire.h>
#include <SPI.h>



// PROGRAMMER VARS | vars for the programmer
bool debug_mode = false; // remove these comparisons for production

// PROGRAM VARS | vars generally required for the program
uint64_t starting_time = 0;

// SENSOR VARS | vars handling sensor data
float kx134_accel_x = FLO_DEF;
float kx134_accel_y = FLO_DEF;
float kx134_accel_z = FLO_DEF;
float ms5611_temp   = FLO_DEF;
float ms5611_press  = FLO_DEF;

// STATE VARS | vars that are important for the state machine
float absolute_altitude = FLO_DEF;
int   decent_check      = INT_DEF;
float rocket_altitude   = 0.0;

// LIMIT VARS | vars defining important limits and thresholds
// NOTE: LIFTOFF_THRESHOLD could be 2 m/s^2, test to see what works best
#define LIFTOFF_THRESHOLD 9.9//1.15f (had 15)



// STATE MACHINE
struct rocket_state
    {
    bool ground_idle = false;
    bool powered_flight = false;
    bool unpowered_flight = false;
    bool ballistic_descent = false;
    bool chute_descent = false;
    bool land_safe = false;
    } rocket;



int init_all() 
    {
    bool all_valid = false;

    while (all_valid == false)
        {
        init_kx134();
        init_MS5611();
        init_fram();
        init_SD();

        // TODO: clarify and condense the following block
        all_valid = true;
        if (all_valid == true) 
            {   
            rocket.ground_idle = true;
            }
        }
    return EXIT_SUCCESS;
    }

int health_check()
    {
    // ! TODO perform sensor checks
    return EXIT_SUCCESS;
    }

void ground_idle_mode(bool state)
    {
    if (state == true)
        {
        if (debug_mode == true) 
            {
            Serial.println("[ROCKET STATE] GROUND IDLE");
            }

        kx134_accel_x = get_kx134_accel_x();
        kx134_accel_y = get_kx134_accel_y();
        kx134_accel_z = get_kx134_accel_z();
    
        if (abs(kx134_accel_z) > LIFTOFF_THRESHOLD) // ? why take abs()
            {
            // START TIMER: starting time is always 0 when running the code for the first time, if this is true set the starting time to the current time
            if (starting_time == 0UL)
                {
                starting_time = millis();
                }

            // DEBUG: Change from 2000 to 100
            // new time - starting time > 0.1 sec and accelation > threshold
            if ((millis() - starting_time > 100) && (abs(kx134_accel_z) > LIFTOFF_THRESHOLD))
                {
                // reset the timer and go to next state
                starting_time = 0UL;
                rocket.powered_flight = true;
                rocket.ground_idle = false;
                }
            // Otherwise restart the starting time since there was an issue
            else
                {
                starting_time = 0UL;
                }
            }
        }
    }


void powered_flight_mode(bool state)
    {
    if (state == true)
        {
        if (debug_mode == true) 
            {
            Serial.println("[ROCKET STATE] POWERED FLIGHT");
            }

        if (abs(kx134_accel_z) < LIFTOFF_THRESHOLD)
            {
            // START TIMER: starting time is always 0 when running the code for the first time, if this is true set the starting time to the current time
            if (starting_time == 0UL)
                {
                starting_time = millis();
                }

            // new time - starting time > 0.1 sec and accelation > threshold
            // DEBUG: Change from 5000 to 100
            if ((millis() - starting_time > 100) && (abs(kx134_accel_z) < LIFTOFF_THRESHOLD))
                {
                  // reset the timer and go to next state
                starting_time = 0;
                rocket.unpowered_flight = true;
                rocket.powered_flight = false;
                }
            }
          }
    }

void apogee_check() 
    {
    //use pressure sensor to check for apogee
    // can also compare using timer
    // example: current_alt < (alt-1sec) -> Descending
    float last_alt = absolute_altitude;

    if (decent_check > 10) 
        {
        rocket.ballistic_descent = true;
        rocket.unpowered_flight = false;
        }

    // GET BMP data on this line
    if ((last_alt - absolute_altitude) > 2) // ? this doesnt make sense, wouldnt it always be 0
        {
        decent_check += decent_check;
        }
    }

void unpowered_flight_mode(bool state)
    {
    if (state == true)
        {
        if (debug_mode == true) 
            {
            Serial.println("[ROCKET STATE] UNPOWERED FLIGHT");
            }
        apogee_check();
        }
    }

void ballistic_descent_mode(bool state)
    {
    if (state == true)
        {
        if (debug_mode == true) 
            {
            Serial.println("[ROCKET STATE] BALLISTIC DESCENT");
            }

        // 1000 ft = 304.8 m
        // Add a backup deployment height
        if (rocket_altitude < 304.8)
            {
            // START TIMER: starting time is always 0 when running the code for the first time
            if (starting_time == 0UL)
                {
                starting_time = millis();
                }
            // new time - starting time > 0.1 sec and accelation > threshold
            if ( (millis() - starting_time > 100) && (rocket_altitude < 304.8))
                {
                // reset the timer and go to next state
                starting_time = 0;
                rocket.chute_descent = true;
                rocket.ballistic_descent = false;
                }
            }
        }
    }

void chute_descent_mode(bool state)
    {
    if (state == true)
        {
        // DEPLOY PARACHUTE
        if (rocket_altitude < 5)
            {
            // START TIMER
            // START TIMER: starting time is always 0 when running the code for the first time
            if (starting_time == 0UL)
                {
                starting_time = millis();
                }

            // new time - starting time > 0.1 sec and accelation > threshold
            if ((millis() - starting_time > 100) && (rocket_altitude < 5))
                {
                // reset the timer and go to next state
                starting_time = 0;
                rocket.land_safe = true;
                rocket.chute_descent = false;
                }
            }
        }
    }

void land_safe_mode(bool state)
    {
    if (state == true)
        {
        // STOP DATA COLLECTION
        // CHECK IF SD CARD CAN STILL BE WRITTEN TO
        // IF SD CARD CAN BE WRITTEN TO AND FLASHCHIP OK
        // WRITE TO SD CARD
        write_to_sd_card("[ROCKET] landed");
        }
    }

void watchdog_callback()
    {
    Serial.println("watchdog_callback()");
    write_to_sd_card("[MICROCONTROLLER] watchdog callback");
    }

void debug_data(bool time_delay)
    {
    // debug true then add delay
    if (time_delay == true)
        {
        delay(500);
        }

    Serial.println("--- Rust lib ---");
    // get pointer and array checks
    // Serial.println(wrap_temperature_for_writing(0));
    // Serial.println(wrap_temperature_for_writing(100));
    // int32_t x = -10;
    // Serial.print("pass and return ");
    // Serial.println(pass_and_return_through_ffi(x));
    // Serial.println(wrap_temperature_for_writing(x));
    // Serial.println(wrap_temperature_for_writing(5.9));
    // Serial.println("called delay func");
    // Serial.println(c_return_delay_test());

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

    // TODO: configure watchdog for error handling
    config.trigger = 1; /* in seconds, 0->128 */
    config.timeout = 2; /* in seconds, 0->128 */
    config.callback = watchdog_callback;
    wdt.begin(config);

    init_all();
    if (health_check() == EXIT_FAILURE)
        {
        Serial.println("[FAILED] Health Check"); // also write to reserved fram space
        exit(1); // this should also fail if init_all() fails;
        }

    Serial.println("setup()");
    write_to_sd_card("setup exit");
    }

void loop() 
    {
    wdt.feed();
    debug_data(true); // remove on prod;

    ground_idle_mode(rocket.ground_idle);
    powered_flight_mode(rocket.powered_flight);
    unpowered_flight_mode(rocket.unpowered_flight);
    ballistic_descent_mode(rocket.ballistic_descent);
    chute_descent_mode(rocket.chute_descent);
    land_safe_mode(rocket.land_safe);
    }