/* NOT USING THESE RIGHT NOW; JUST TESTING OUT A MVP*/
//#include "kalmanFilter1dconst.h"
//#include "BasicLinearAlgebra.h"
//#include "movingAverage.h"
//using namespace BLA;
//#include "Filter.h"
#include "default_variables.h"
#include "parachuteDeploy.h"
#include "rocket_profile.h"
#include "sensor_ms5611.h"
#include "sensor_bmi088.h"
#include "sensor_bmp280.h"
#include "sensor_sdcard.h"
#include "sensor_kx134.h"
#include "sensor_radio.h"
#include "statemachine.h"
#include "memory_fram.h"
#include "errorcodes.h"
#include "sensor_led.h"
#include "rusty_fram.h"
#include "I2CScanner.h"
#include "watchdog.h"
#include <Arduino.h>
#include <RH_RF95.h>
#include <stdint.h> // switch to machine independent types
#include <stdlib.h>
#include <string>
#include <Wire.h>
#include <SPI.h>


// PROGRAMMER VARS | vars for the programmer
unsigned long debug_time = 0UL;
bool debug_mode = false; // remove these comparisons for production
I2CScanner scanner;

// PROGRAM VARS | vars generally required for the program
unsigned long starting_time = 0UL;

// SENSOR VARS | vars handling sensor data
float kx134_accel_x = FLO_DEF;
float kx134_accel_y = FLO_DEF;
float kx134_accel_z = FLO_DEF;
float ms5611_temp   = FLO_DEF;
float ms5611_press  = FLO_DEF;

// STATE VARS | vars that are important for the state machine

// LIMIT VARS | vars defining important limits and thresholds
#define LIFTOFF_THRESHOLD 15.0f // confirm with propulsion, nominal is 8.5 to 10.5



// STATE MACHINE
static statemachine::e_rocket_state rocket_state;


int init_all()
    {
    init_kx134();
    init_MS5611();
    init_bmp280();
    init_SD();
    init_fram();
    init_LED();

    // TODO:
    // init_bmi088();
    // init_RFM95_TX();

    ground_base_pressure = get_bmp280_pressure();
    ground_base_altitude = get_bmp280_altitude(ground_base_pressure);
    rocket_state = statemachine::e_rocket_state::unarmed;

    write_to_sd_card("time, state,kx134_x, kx134_y, kx134_z, bmp280_alt");

    return EXIT_SUCCESS;
    }

int health_check()
    {
    // KX134 checks
    float z_thresh_low = 9.0;
    float z_thresh_high = 10.0;
    float curr_z_reading = get_kx134_accel_z();
    int count = 0;

    while (count < 10)
        {
        if (curr_z_reading < z_thresh_high && curr_z_reading > z_thresh_low)
            {
            count++;
            }
        else
            {
            return EXIT_FAILURE;
            }
        }

    // BMP280
    float alt_thresh_low = 0.0;
    float alt_thresh_high = ROCKET_HEIGHT;
    float curr_alt_reading = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
    count = 0;

    while (count < 10)
        {
        if (curr_alt_reading > alt_thresh_low && curr_alt_reading < alt_thresh_high)
            {
            count++;
            }
        else
            {
            return EXIT_FAILURE;
            }
        }


    return EXIT_SUCCESS;
    }

void ground_idle_mode()
    {
    if (debug_mode == true) 
        {
        Serial.println("[ROCKET STATE] GROUND IDLE");
        }

    setLedGreen();
    // TODO:
    // buzzerON(0); play state ok sound

    kx134_accel_z = get_kx134_accel_z();
    if (starting_time == 0)
        {
        starting_time = millis();
        }

    if (((millis() - starting_time) > 250) && (get_kx134_accel_z() - kx134_accel_z) > LIFTOFF_THRESHOLD)
        {
        starting_time = 0UL;
        rocket_state = statemachine::e_rocket_state::powered_flight;
        }
    }


void powered_flight_mode()
    {
    if (debug_mode == true) 
        {
        Serial.println("[ROCKET STATE] POWERED FLIGHT");
        }

    setLedRed();
    // buzzerON(1);

    // TODO: add to apogee buffer

    /*
    powered to unpowered flight is typical to deceleration
    */

    kx134_accel_z = get_kx134_accel_z();
    if (starting_time == 0)
        {
        starting_time = millis();
        }

    if ((millis() - starting_time > 250) &&(kx134_accel_z - get_kx134_accel_z()) > (LIFTOFF_THRESHOLD / 2))
        {
        starting_time = 0UL;
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

    setLedBlue();

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
    
    rocket_altitude = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);

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
        get_start_signal_from_ground_station(rs); // not armed until ground station tells it to!
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
    loop();
    }

int debug_data()
    {
    String data_string = ""; 

    // debug true then add delay
    if (debug_time == 0UL)
        {
        debug_time = millis();
        }
 
    if ((millis() - debug_time) < 500)
        {
        return EXIT_FAILURE;
        }

    data_string += millis();
    data_string += rocket_state;

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

    data_string += kx134_accel_x;
    data_string += kx134_accel_y;
    data_string += kx134_accel_z;

    // MS5611
    Serial.println("--- MS5611 ---");
    ms5611_temp = get_ms5611_temp();
    ms5611_press = get_ms5611_press();
    Serial.print("temperature: ");
    Serial.println(ms5611_temp);
    Serial.print("pressure: ");
    Serial.println(ms5611_press);

    // BMP280
    Serial.println("---BMP280---");
    Serial.print("ground base pressure: ");
    Serial.println(ground_base_pressure);
    Serial.print("altitude: ");
    float bmp280_altitude = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
    Serial.println(bmp280_altitude);

    data_string += bmp280_altitude;

    // Rocket State enum
    Serial.println("---Enum Rocket State---");
    Serial.print("current enum state: ");
    Serial.println(rocket_state);
    if (rocket_state == statemachine::ground_idle){ Serial.println("if detected rocket state to be ground idle"); }
    if (rocket_state == statemachine::powered_flight){ Serial.println("if detected rocket state to be powered flight"); }
    if (rocket_state == statemachine::unpowered_flight){ Serial.println("if detected rocket state to be unpowered flight"); }
    if (rocket_state == statemachine::ballistic_descent){ Serial.println("if detected rocket state to be ballistic decent"); }
    if (rocket_state == statemachine::chute_descent){ Serial.println("if detected rocket state to be chute descent"); }
    if (rocket_state == statemachine::land_safe){ Serial.println("if detected rocket state to be land safe"); exit(1);}

    scanner.Scan();
    return EXIT_SUCCESS;
    }

// STANDARD ENTRY POINTS
void setup() 
    {
    Serial.begin(9600); // arg doesnt need to be 9600 just true
    Wire.begin();

    config.trigger = 2; /* in seconds, 0->128 */
    config.timeout = 3; /* in seconds, 0->128 */
    config.callback = watchdog_callback;

    init_all();

    if (health_check() == EXIT_FAILURE)
        {
        Serial.println("[FAILED] Health Check"); // also write to reserved fram space
        exit(1); // this should also fail if init_all() fails;
        }

    Serial.println("setup()");
    write_to_sd_card("setup exit");

    wdt.begin(config);
    wdt.feed();
    }

void loop() 
    {
    // this function flashes an internal led
    flashInternalLed(true);
    wdt.feed();
    debug_data(); // remove on prod;
    select_flight_mode(rocket_state);
    flashInternalLed(false);
    }