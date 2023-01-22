#include "package_testmode.h"
#include "package_watchdog.h"

#include "sensor_parachute.h"
#include "sensor_buzzer.h"
#include "sensor_ms5611.h"
#include "sensor_bmi088.h"
#include "sensor_sdcard.h"
#include "sensor_bmp280.h"
#include "sensor_kx134.h"
#include "sensor_radio.h"
#include "sensor_fram.h"
#include "sensor_led.h"

#include "default_variables.h"
#include "rocket_profile.h"
#include "user_variables.h"
#include "statemachine_t.h"

#include "debug_I2C_Scanner.h"

#include "RFM95W.h"

#include <Arduino.h>
#include <stdint.h> // switch to machine independent types
#include <stdlib.h>
#include <Wire.h>
#include <SPI.h>



// PROGRAM VARS | vars generally required for the program
unsigned long starting_time = 0UL;
int descent_check = 0;
float last_alt = 0;


// STATE MACHINE
static statemachine_t::e_rocket_state rocket_state;

static RFM95W Radio(10, 31, 32);

int init_all()
    {
    init_kx134();
    init_MS5611();
    //init_bmp280();
    init_SD();
    // init_fram();
    init_LED();

    Radio.Set_Frequency(914.1);

    // TODO:
    // init_bmi088();
    // init_RFM95_TX();

    ms5611_ground_base_pressure = get_ms5611_press();
    //ground_base_pressure = get_bmp280_pressure();
    //ground_base_altitude = get_bmp280_altitude(ground_base_pressure);
    rocket_state = statemachine_t::e_rocket_state::unarmed;

    #ifdef ROCKET_DEBUGMODE
        rocket_state = enter_state(STATE_TO_ENTER);
    #endif

    write_to_sd_card(DATALOG, datalog_fmt_header);

    return EXIT_SUCCESS;
    }

int health_check()
    {
    println("health_check()");

    // KX134 checks
    float z_thresh_low = 9.0;
    float z_thresh_high = 11.0;
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
    // float alt_thresh_low = -0.25;
    // float alt_thresh_high = 0.50;
    //float curr_alt_reading = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
    // float curr_alt_reading = get_ms5611_altitude(get_ms5611_press(), ms5611_ground_base_pressure);
    count = 0;
   /* while (count < 10)
        {
        if (curr_alt_reading > alt_thresh_low && curr_alt_reading < alt_thresh_high)
            {
            count++;
            }
        else
            {
            return EXIT_FAILURE;
            }
        }*/

    write_to_sd_card(EVENTLOG, "health checks passed");

    return EXIT_SUCCESS;
    }

void ground_idle_mode()
    {
    println("[ROCKET STATE] GROUND IDLE");

    setLedGreen();

    if (starting_time == 0)
        {
        starting_time = millis();
        }

    if (millis() - starting_time > 500)
        {
        starting_time = 0UL;
        buzzerOn();
        }

    kx134_accel_z = get_kx134_accel_z();

    if ((starting_time == 0) && (kx134_accel_z > LIFTOFF_THRESHOLD))
        {
        starting_time = millis();
        }

    if (((millis() - starting_time) > 250) && (kx134_accel_z > LIFTOFF_THRESHOLD))
        {
        starting_time = 0UL;
        rocket_state = statemachine_t::e_rocket_state::powered_flight;
        }
    }

void powered_flight_mode()
    {
    println("[ROCKET STATE] POWERED FLIGHT");

    setLedRed();
    // buzzerON(1);

    // TODO: add to apogee buffer

    /*
    powered to unpowered flight is typical to deceleration
    */
    kx134_accel_z = get_kx134_accel_z();

    if ((starting_time == 0) && ((kx134_accel_z) < LIFTOFF_THRESHOLD))
        {
        starting_time = millis();
        }

    if ((millis() - starting_time > 100) && ((kx134_accel_z) < (LIFTOFF_THRESHOLD)))
        {
        starting_time = 0UL;
        rocket_state = statemachine_t::e_rocket_state::unpowered_flight;
        }

    }

bool apogee_check() 
    {
    if (descent_check > DESCENT_CHECK_AMOUNT)
        {
        return true;
        }
    else if (starting_time == 0)
        {
        //last_alt = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
        last_alt = get_ms5611_altitude(get_ms5611_press(), ms5611_ground_base_pressure);
        starting_time = millis();
        }

    else if ((millis() - starting_time > 100) && ((last_alt - (get_ms5611_altitude(get_ms5611_press(), ms5611_ground_base_pressure))) > ALTITUDE_CHANGE))
        {
        starting_time = 0UL;
        descent_check++;
        }
    else if (millis() - starting_time > 5000)
        {
        starting_time = 0UL;
        }

    return false;
    }

void unpowered_flight_mode()
    {
    println("[ROCKET STATE] UNPOWERED FLIGHT");

    setLedBlue();

    if (apogee_check() == true)
        {
        rocket_state = statemachine_t::e_rocket_state::ballistic_descent;
        }
    }

void ballistic_descent_mode()
    {
    println("[ROCKET STATE] BALLISTIC DESCENT");

    // ledON("YELLOW");

    // TODO: Add a backup deployment height
    rocket_altitude = get_ms5611_altitude(get_ms5611_press(), ms5611_ground_base_pressure);

    if (rocket_altitude <= PARACHUTE_DEPLOYMENT_HEIGHT)
        {
        deploy_parachute();
        rocket_state = statemachine_t::e_rocket_state::chute_descent;
        }
    }

void chute_descent_mode()
    {
    // TODO:
    // ledON("ORANGE");
         
    // TODO: check gyroscope stabilisation over time
    rocket_altitude = get_ms5611_altitude(get_ms5611_press(), ms5611_ground_base_pressure);
    if (rocket_altitude<LANDING_ALTITUDE)
        {
        rocket_state = statemachine_t::e_rocket_state::land_safe;
        }

    }

void land_safe_mode()
    {
    // STOP DATA COLLECTION
    // CHECK IF SD CARD CAN STILL BE WRITTEN TO
    // IF SD CARD CAN BE WRITTEN TO AND FLASHCHIP OK
    // WRITE TO SD CARD
    write_to_sd_card(EVENTLOG, "[ROCKET] Landed");
    if (starting_time == 0)
        {
        starting_time = millis();
        }

    if (millis() - starting_time > 500)
        {
        starting_time = 0UL;
        buzzerOn();
        }
        // TODO: call on func to read, unzip and write date to SD card
        // ledON(somecolour);
    }

int select_flight_mode(statemachine_t::e_rocket_state &rs)
    {
    switch (rs)
        {
        case statemachine_t::unarmed:
            get_start_signal_from_ground_station(rs); // not armed until ground station tells it to!
            break;
        case statemachine_t::ground_idle:
            ground_idle_mode();
            break;
        case statemachine_t::powered_flight:
            powered_flight_mode();
            break;
        case statemachine_t::unpowered_flight:
            unpowered_flight_mode();
            break;
        case statemachine_t::ballistic_descent:
            ballistic_descent_mode();
            break;
        case statemachine_t::chute_descent:
            chute_descent_mode();
            break;
        case statemachine_t::land_safe:
            land_safe_mode();
            break;
        case statemachine_t::test:
            test_mode_state();
            break;
        default:
            test_mode_state();
            break;
        }
    return EXIT_FAILURE;
    }

void watchdog_callback()
    {
    println("watchdog_callback()");
    write_to_sd_card(EVENTLOG, "[MICROCONTROLLER] watchdog callback");
    loop();
    }

int debug_data()
    {
    #ifdef ROCKET_DEBUGMODE

    String data_string = "";

    if (debug_time == 0UL)
        {
        debug_time = millis();
        }

    if ((millis() - debug_time) < 1000)
        {
        return EXIT_FAILURE;
        }

    data_string += String(millis()) + ",";
    data_string += String(rocket_state) + ",";

    kx134_accel_x = get_kx134_accel_x();
    kx134_accel_y = get_kx134_accel_y();
    kx134_accel_z = get_kx134_accel_z();
    data_string = data_string + String(kx134_accel_x) + ",";
    data_string = data_string + String(kx134_accel_y) + ",";
    data_string = data_string + String(kx134_accel_z) + ",";

    rocket_altitude = get_ms5611_altitude(get_ms5611_press(), ms5611_ground_base_pressure);
    data_string = data_string + String(rocket_altitude);

    write_to_sd_card(DATALOG, data_string.c_str());

    print("data_string: ");
    println(data_string);

    scan_and_print_I2C_devices();

    print("rocket state: ");
    println(rocket_state);

    #endif

    return EXIT_SUCCESS;
    }

// this can go anywhere
double Exponential_moving_averages(double Data, double Previous_data, float Weight)
{
    return Data * Weight + Previous_data * (1 - Weight);
}

// STANDARD ENTRY POINTS
void setup() 
    {
    Serial.begin(9600); // arg doesnt need to be 9600 just true
    Wire.begin();

    // config.trigger = 2; /* in seconds, 0->128 */
    // config.timeout = 3; /* in seconds, 0->128 */
    // config.callback = watchdog_callback;

    init_all();


    if (health_check() == EXIT_FAILURE)
        {
        println("[FAILED] Health Check"); // also write to reserved fram space
        exit(1); // this should also fail if init_all() fails;
        }

    println("setup()");
    write_to_sd_card(EVENTLOG, "setup exit");
    buzzerOn();

    // wdt.begin(config);
    // wdt.feed();

    // TODO: add a method to read previous state from FRAM and restore it
    // int value_from_fram = read_from_fram(0x0);
    // rocket_state = set_state_for_statemachine(&rocket_state, value_from_fram);



    }

void loop() 
    {
    Radio.UDP_Send("Hello from the other side");
    // wdt.feed();
    debug_data();
    select_flight_mode(rocket_state);
    }