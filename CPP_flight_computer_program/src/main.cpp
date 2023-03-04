#include "package_fram.h"
#include "package_statemachine_t.h"
#include "package_statistics.h"
#include "package_testmode.h"
#include "package_watchdog.h"

#include "sensor_bmi088.h"
#include "sensor_bmp280.h"
#include "sensor_buzzer.h"
#include "sensor_kx134.h"
#include "sensor_led.h"
#include "sensor_parachute.h"
#include "sensor_radio.h"
#include "sensor_thermocouple.h"

#include "rocket_profile.h"
#include "statemachine_t.h"

#include "debug_I2C_Scanner.h"
#include "debug_macros.h"

#include "test_main.h"

#include <Arduino.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <Wire.h>
#include <cstdint> // switch to machine independent types
#include <cstdlib>
#include <limits>


// PROGRAM VARS | vars generally required for the program
unsigned long starting_time = 0UL;
unsigned long debug_time    = 0UL;
int descent_check           = 0;
float last_alt              = 0;
float apogee_buffer[APOGEE_BUFFER_SIZE];
float time_buffer[APOGEE_BUFFER_SIZE];
uint8_t apogee_buffer_cursor = 0;

// STATE MACHINE
static statemachine_t::e_rocket_state rocket_state;

// GLOBAL VARS
float ground_base_pressure = 0.0f;
float ground_base_altitude = 0.0f;
float rocket_altitude      = 0.0f;

float kx134_accel_x        = 0.0f;
float kx134_accel_y        = 0.0f;
float kx134_accel_z        = 0.0f;

float rocket_apogee        = 0.0f;



int init_all()
	{
	init_kx134();
	init_bmp280();
	init_fram_package();

	// init_LED();

	// TODO:
	// init_bmi088();
	// init_RFM95_TX();

	// init_I2C_scanner();

	// Setting global variables
	ground_base_pressure = get_bmp280_pressure();
	ground_base_altitude = get_bmp280_altitude(ground_base_pressure);
	rocket_state         = statemachine_t::e_rocket_state::unarmed;

	// zombie pin init
	pinMode(PIN_A14, INPUT_PULLUP);

#ifdef ROCKET_DEBUGMODE
	rocket_state = set_current_state_for_statemachine(rocket_state, GROUND_IDLE_STATE);
#endif

	// write_to_sd_card(DATALOG, datalog_fmt_header);

	return EXIT_SUCCESS;
	}

int health_check()
	{
	return EXIT_SUCCESS; // TODO: implement health checks and delete this line

	println("health_check()");

	// KX134 checks
	float z_thresh_low   = 9.0;
	float z_thresh_high  = 11.0;
	float curr_z_reading = get_kx134_accel_z();

	int count            = 0;
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

	// BMP280 checks
	float alt_thresh_low   = -0.25;
	float alt_thresh_high  = 0.50;
	float curr_alt_reading = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);

	count                  = 0;
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

	// FRAM checks


	// write_to_sd_card(EVENTLOG, "health checks passed");

	return EXIT_SUCCESS;
	}

void ground_idle_mode()
	{
	//println("[ROCKET STATE] GROUND IDLE");

	setLedGreen();
	//println("TEST");
	if (starting_time == 0)
		{
		starting_time = millis();
		}

	if (millis() - starting_time > 500)
		{
		starting_time = 0UL;
		buzzer_on();
		}

	kx134_accel_z = get_kx134_accel_z();

	if ((starting_time == 0) && (kx134_accel_z > LIFTOFF_THRESHOLD))
		{
		starting_time = millis();
		}

	if (((millis() - starting_time) > 250) && (kx134_accel_z > LIFTOFF_THRESHOLD))
		{
		starting_time = 0UL;
		rocket_state  = statemachine_t::e_rocket_state::powered_flight;
		}
	}

void powered_flight_mode()
	{
	//println("[ROCKET STATE] POWERED FLIGHT");

	setLedRed();

	// powered to unpowered flight is typical of deceleration
	kx134_accel_z = get_kx134_accel_z();

	if ((starting_time == 0) && ((kx134_accel_z) < LIFTOFF_THRESHOLD))
		{
		starting_time = millis();
		}

	if ((millis() - starting_time > 100) && ((kx134_accel_z) < (LIFTOFF_THRESHOLD)))
		{
		starting_time = 0UL;
		rocket_state  = statemachine_t::e_rocket_state::unpowered_flight;
		}
	}

/*
 * @note - this function will take a little under a second to run
 */
int apogee_check()
	{
		apogee_buffer[apogee_buffer_cursor] = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
		time_buffer[apogee_buffer_cursor] = millis();
		apogee_buffer_cursor++;
		if (apogee_buffer_cursor >= APOGEE_BUFFER_SIZE)
			{
				apogee_buffer_cursor = 0;
				int count = APOGEE_BUFFER_SIZE;
				int sum_x = 0;
				int sum_y = 0;
				int sum_x2 = 0;
				int sum_xy = 0;
				for (int i=0;i<APOGEE_BUFFER_SIZE; i++)
				{
					sum_x += time_buffer[i];
					sum_y += apogee_buffer[i];
					sum_x2 += (time_buffer[i]*time_buffer[i]);
					sum_xy += (time_buffer[i]*apogee_buffer[i]);
				}
				int x_mean = sum_x/count;
				int y_mean = sum_y/count;
				int slope = ( sum_xy - (sum_x * y_mean) ) / (sum_x2 - (sum_x * x_mean) );
				//print(slope);
				//print(",");
				if (slope <= 0)
				{
					return EXIT_SUCCESS;
				}
				
			}
			return EXIT_FAILURE;
	}

void unpowered_flight_mode()
	{
	//println("[ROCKET STATE] UNPOWERED FLIGHT");

	setLedBlue();

	if (apogee_check() == EXIT_SUCCESS)
		{
		rocket_state = statemachine_t::e_rocket_state::ballistic_descent;
		}
	}

void soft_recovery_mode()
	{
	//println("[ROCKET STATE] SOFT RECOVERY");

	while (true)
		{
		wdt.feed(); // feeding the watchdog to avoid a reset
		int apogee_check_result = apogee_check();
		if (apogee_check_result == EXIT_SUCCESS)
			{
			println("apogee detected!");
			while (true)
				{
				wdt.feed(); // feeding the watchdog to avoid a reset
				if (get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude) < PARACHUTE_DEPLOYMENT_HEIGHT)
					{
					deploy_parachute();
					rocket_state = statemachine_t::e_rocket_state::chute_descent;
					break;
					}
				}
			break;
			}
		}
	loop(); // restart the loop
	}

void ballistic_descent_mode()
	{
	//println("[ROCKET STATE] BALLISTIC DESCENT");

	// ledON("YELLOW");

	// TODO: Add a backup deployment height
	rocket_altitude = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);

	if (rocket_altitude <= PARACHUTE_DEPLOYMENT_HEIGHT)
		{
		deploy_parachute();
		rocket_state = statemachine_t::e_rocket_state::chute_descent;
		}
	}

void chute_descent_mode()
	{
	//println("[ROCKET STATE] CHUTE DESCENT");
	// TODO:
	// ledON("ORANGE");

	// TODO: check gyroscope stabilisation over time
	rocket_altitude = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);

	if (rocket_altitude < LANDING_ALTITUDE)
		{
		rocket_state = statemachine_t::e_rocket_state::land_safe;
		}
	}

void land_safe_mode()
	{
	//println("[ROCKET STATE] LAND SAFE");
	// STOP DATA COLLECTION
	// CHECK IF SD CARD CAN STILL BE WRITTEN TO
	// IF SD CARD CAN BE WRITTEN TO AND FLASHCHIP OK
	// WRITE TO SD CARD
	// write_to_sd_card(EVENTLOG, "[ROCKET] Landed");
	if (starting_time == 0)
		{
		starting_time = millis();
		}

	if (millis() - starting_time > 500)
		{
		starting_time = 0UL;
		buzzer_on();
		}
	// TODO: call on func to read, unzip and write date to SD card
	// ledON(somecolour);
	}

int select_flight_mode(statemachine_t::e_rocket_state& rs)
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
	case statemachine_t::soft_recovery:
		soft_recovery_mode();
		break;
	case statemachine_t::test_state:
		// TODO: test_mode_state();
		break;
	default:
		soft_recovery_mode();
		break;
		}
	return EXIT_FAILURE;
	}

void watchdog_callback()
	{
	wdt.feed();
	println("watchdog_callback()");
	// write_to_sd_card(EVENTLOG, "[MICROCONTROLLER] watchdog callback");
	rocket_state = statemachine_t::e_rocket_state::soft_recovery;
	loop();
	}

int check_zombie_mode()
	{
	if (digitalRead(PIN_A14) == HIGH)
		{
		return EXIT_SUCCESS;
		}
	return EXIT_FAILURE;
	}

int debug_data()
	{
#ifdef ROCKET_DEBUGMODE

	String data_string_fmt = "millis(), rocket_state, kx134_accel_x, kx134_accel_y, kx134_accel_z, relative_altitude";
	String data_string     = "";

	if (debug_time == 0UL)
		{
		debug_time = millis();
		}

	/*if ((millis() - debug_time) < DEBUG_INTERVAL)
		{
		return EXIT_FAILURE;
		}*/

	data_string += String(millis()) + ",";
	data_string += String(rocket_state) + ",";

	kx134_accel_x   = get_kx134_accel_x();
	kx134_accel_y   = get_kx134_accel_y();
	kx134_accel_z   = get_kx134_accel_z();
	data_string     = data_string + String(kx134_accel_x) + ",";
	data_string     = data_string + String(kx134_accel_y) + ",";
	data_string     = data_string + String(kx134_accel_z) + ",";

	rocket_altitude = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
	data_string     = data_string + String(rocket_altitude);

	// write_to_sd_card(DATALOG, data_string.c_str());

	//println(data_string_fmt);
	//print("data_string: ");
	println(data_string);

	// scan_and_print_I2C_devices();

	debug_time = 0UL;

#endif // ROCKET_DEBUGMODE

	return EXIT_SUCCESS;
	}

// STANDARD ENTRY POINTS
void setup()
	{
	Serial.begin(9600);
	Wire.begin();

#if (TESTMODE == 1)
	#warning "TESTMODE ENABLED"
	#warning "only tests will be run, no rocket code"
	println("[TESTMODE] testing Teensy 4.1");
	test_main();
#endif // TESTMODE

	init_all();

	// zombie mode ensures that the main loop()
	// does not start running, overwriting previous
	// data on the FRAM
	/*if (check_zombie_mode() == EXIT_SUCCESS)
		{
		println("[ZOMBIE MODE] detected");
		dump_fram_to_serial();
		exit(0);
		}*/

	if (health_check() == EXIT_FAILURE)
		{
		println("[FAILED] Health Check"); // also write to reserved fram space
		exit(1);                          // this should also fail if init_all() fails;
		}

	buzzer_on();

	config.trigger  = WATCHDOG_TRIGGER;
	config.timeout  = WATCHDOG_TIMEOUT;
	config.callback = watchdog_callback;
	wdt.begin(config);
	wdt.feed();

	// TODO: add a method to read previous state from FRAM and restore it
	println("setup() exit");
	}

void loop()
	{
	float notanumber = std::numeric_limits<float>::quiet_NaN();
	wdt.feed();
	debug_data();
	select_flight_mode(rocket_state);
	write_data_chunk_to_fram(
	    millis(), rocket_state,
	    kx134_accel_x, kx134_accel_y, kx134_accel_z,
	    notanumber, notanumber, notanumber,
	    rocket_altitude, get_bmp280_pressure(), get_thermocouple_external_temperature());
	}
