/* NOT USING THESE RIGHT NOW; JUST TESTING OUT A MVP*/
//#include "kalmanFilter1dconst.h"
//#include "BasicLinearAlgebra.h"
//#include "movingAverage.h"
//using namespace BLA;
//#include "Filter.h"
#include "default_variables.h"
#include "parachuteDeploy.h"
#include "sensor_ms5611.h"
#include "sensor_bmp280.h"
#include "sensor_sdcard.h"
#include "sensor_kx134.h"
#include "sensor_radio.h"
#include "statemachine.h"
#include "memory_fram.h"
#include "errorcodes.h"
#include "rusty_fram.h"
#include "watchdog.h"
#include <Arduino.h>
#include <RH_RF95.h>
#include <cstdint> // switch to machine independent types
#include <cstdlib>
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
float ground_base_pressure = FLO_DEF;

// STATE VARS | vars that are important for the state machine
float absolute_altitude = FLO_DEF;
float rocket_altitude   = 0.0;

// LIMIT VARS | vars defining important limits and thresholds
// NOTE: LIFTOFF_THRESHOLD could be 2 m/s^2, test to see what works best
#define LIFTOFF_THRESHOLD 10.0f // confirm with propulsion
#define PARACHUTE_DEPLOYMENT_HEIGHT 350



// STATE MACHINE
static statemachine::e_rocket_state rocket_state;


int init_all()
    {
    init_kx134();
    init_MS5611();
    init_fram();
    init_SD();

    // TODO:
    // init_bni088();
    // init_LED();
    // init_RFM95_TX();

    ground_base_pressure = get_bmp280_pressure();
    rocket_state = statemachine::e_rocket_state::unarmed;

    return EXIT_SUCCESS;
    }

int health_check()
    {
    // TODO: perform sensor checks
    return EXIT_SUCCESS;
    }

void ground_idle_mode()
    {
    if (debug_mode == true) 
        {
        Serial.println("[ROCKET STATE] GROUND IDLE");
        }

    // TODO:
    // ledON("GREEN");
    // buzzerON(0); play state ok sound

    kx134_accel_z = get_kx134_accel_z();
    delay(500);

    if ((get_kx134_accel_z() - kx134_accel_z) > LIFTOFF_THRESHOLD)
        {
        rocket_state = statemachine::e_rocket_state::powered_flight;
        }
    }


void powered_flight_mode()
    {
    if (debug_mode == true) 
        {
        Serial.println("[ROCKET STATE] POWERED FLIGHT");
        }

    // ledON("RED");
    // buzzerON(1);

    // TODO: add to apogee buffer

    /*
    powered to unpowered flight is typical to deceleration
    */

    kx134_accel_z = get_kx134_accel_z();
    delay(500);

    if ((kx134_accel_z - get_kx134_accel_z()) > (LIFTOFF_THRESHOLD / 2))
        {
        rocket_state = statemachine::e_rocket_state::unpowered_flight;
        }

    }

bool apogee_check() 
    {
    // deleted decent_check var
    // TODO: create an apogee buffer
    return true;
    }

void unpowered_flight_mode()
    {
    if (debug_mode == true) 
        {
        Serial.println("[ROCKET STATE] UNPOWERED FLIGHT");
        }

        // TODO:
        // ledON("BLUE");

    if (apogee_check() == true)
        {
        rocket_state = statemachine::e_rocket_state::ballistic_descent;
        }
    }

void ballistic_descent_mode()
    {
    if (debug_mode == true) 
        {
        Serial.println("[ROCKET STATE] BALLISTIC DESCENT");
        }

    // TODO:
    // ledON("YELLOW");
    // 1000 ft = 304.8 m
    // Add a backup deployment height
    
    rocket_altitude = get_bmp280_altitude(ground_base_pressure); 

    if (rocket_altitude <= PARACHUTE_DEPLOYMENT_HEIGHT)
        {
        deploy_parachute();
        rocket_state = statemachine::e_rocket_state::chute_descent;
        }
    }

void chute_descent_mode()
    {
    // TODO:
    // ledON("ORANGE");

         
    // TODO: check gyroscope stabilisation over time

    rocket_state = statemachine::e_rocket_state::land_safe;
    }

void land_safe_mode()
    {
        // STOP DATA COLLECTION
        // CHECK IF SD CARD CAN STILL BE WRITTEN TO
        // IF SD CARD CAN BE WRITTEN TO AND FLASHCHIP OK
        // WRITE TO SD CARD
        write_to_sd_card("[ROCKET] Landed");
        // TODO: call on func to read, unzip and write date to SD card
        // ledON(somecolour);
        }

int select_flight_mode(statemachine::e_rocket_state &rs)
    {
    // TODO: change to switch case
    if (rs == statemachine::unarmed)
        {
        get_start_signal_from_ground_station(rs);
        }
    else if (rs == statemachine::ground_idle)
        {
        ground_idle_mode();
        }
    else if (rs == statemachine::powered_flight)
        {
        powered_flight_mode();
        }
    else if (rs == statemachine::unpowered_flight)
        {
        unpowered_flight_mode();
        }
    else if (rs == statemachine::ballistic_descent)
        {
        ballistic_descent_mode();
        }
    else if (rs == statemachine::chute_descent)
        {
        chute_descent_mode();
        }
    else if (rs == statemachine::land_safe)
        {
        land_safe_mode();
        }
    return EXIT_FAILURE;
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

    // Rust FFI lib
    Serial.println("--- Rust lib ---");

    // KX134
    Serial.println("--- KX134 ---");
    kx134_accel_x = get_kx134_accel_x();
    kx134_accel_y = get_kx134_accel_y();
    kx134_accel_z = get_kx134_accel_z();
    Serial.print("x: ");
    Serial.println(kx134_accel_x);
    Serial.print("y: ");
    Serial.println(kx134_accel_y);
    Serial.print("z: ");
    Serial.println(kx134_accel_z);

    // MS5611
    Serial.println("--- MS5611 ---");
    ms5611_temp = get_ms5611_temp();
    ms5611_press = get_ms5611_press();
    Serial.print("temperature: ");
    Serial.println(ms5611_temp);
    Serial.print("pressure: ");
    Serial.println(ms5611_press);

    // Rocket State enum
    Serial.println("---Enum Rocket State---");
    Serial.print("current enum state: ");
    Serial.println(rocket_state);
    if (rocket_state == statemachine::ground_idle){ Serial.println("if detected rocket state to be ground idle"); }
    if (rocket_state == statemachine::powered_flight){ Serial.println("if detected rocket state to be powered flight"); }
    if (rocket_state == statemachine::unpowered_flight){ Serial.println("if detected rocket state to be unpowered flight"); }
    if (rocket_state == statemachine::ballistic_descent){ Serial.println("if detected rocket state to be ballistic decent"); }
    if (rocket_state == statemachine::chute_descent){ Serial.println("if detected rocket state to be chute descent"); }
    if (rocket_state == statemachine::land_safe){ Serial.println("if detected rocket state to be land safe"); }
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
    debug_data(false); // remove on prod;

    wdt.feed();
    select_flight_mode(rocket_state);
    }