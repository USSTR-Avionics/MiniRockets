#include "package_fram.h"
#include "package_statemachine_t.h"
#include "package_statistics.h"
#include "package_testmode.h"
#include "package_watchdog.h"

#include "sensor_bmi088.h"
#include "sensor_bmp280.h"
#include "sensor_buzzer.h"
#include "sensor_fram.h" // TODO: sub with package
#include "sensor_kx134.h"
#include "sensor_led.h"
#include "sensor_parachute.h"
#include "sensor_radio.h"
#include "sensor_sdcard.h"

#include "rocket_profile.h"
#include "statemachine_t.h"

#include "debug_I2C_Scanner.h"
#include "debug_macros.h"

#include "test_main.h"

#include <Arduino.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <Wire.h>
#include <stdint.h> // switch to machine independent types
#include <stdlib.h>



// PROGRAM VARS | vars generally required for the program
unsigned long starting_time = 0UL;
unsigned long debug_time    = 0UL;
int descent_check           = 0;
float last_alt              = 0;


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

	// init_SD();
	// init_fram();

	// init_LED();

	// TODO:
	// init_bmi088();
	// init_RFM95_TX();

	// Setting global variables
	ground_base_pressure = get_bmp280_pressure();
	ground_base_altitude = get_bmp280_altitude(ground_base_pressure);
	rocket_state         = statemachine_t::e_rocket_state::unarmed;

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
	count                   = 0;

	uint8_t test_byte_write = 128;

	while (count < 10)
		{
		write_to_fram(test_byte_write, 0);
		uint8_t test_byte_read = read_from_fram(0);

		if (test_byte_read == test_byte_write)
			{
			count++;
			}
		else
			{
			return EXIT_FAILURE;
			}
		}

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
	println("[ROCKET STATE] POWERED FLIGHT");

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
 * @note - this function will take a little over second to run
 */
int apogee_check()
	{
	// fill buffer with ema value readings then impl every x ms
	starting_time                = millis();
	uint8_t apogee_buffer_cursor = 0;
	float apogee_buffer[APOGEE_BUFFER_SIZE];

	while (true)
		{
		if (millis() - starting_time > APOGEE_READING_INTERVAL)
			{
			if (apogee_buffer_cursor == 0)
				{
				apogee_buffer[apogee_buffer_cursor] = ceilf(get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude) * 100) / 100; // rounding up to 2 decimal places
				}
			else
				{
				float altitude_reading              = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
				float ema_adjusted                  = get_exponential_moving_average(altitude_reading, apogee_buffer[apogee_buffer_cursor - 1], MODERATE_EMA_SMOOTHING);
				apogee_buffer[apogee_buffer_cursor] = ceilf(ema_adjusted * 100) / 100; // rounding up to 2 decimal places
				}

			// increment apogee buffer cursor
			apogee_buffer_cursor++;
			// reset local timer
			starting_time = millis();
			}

		if (apogee_buffer_cursor >= APOGEE_BUFFER_SIZE)
			{
			break;
			}
		}

	// print buffer
	for (int i = 0; i < APOGEE_BUFFER_SIZE; i++)
		{
		print(apogee_buffer[i]);
		print(", ");
		}
	println("");

	// check if monotonically non increasing
	for (int i = 0; i < APOGEE_BUFFER_SIZE; i++)
		{
		if (apogee_buffer[i] < apogee_buffer[i + 1]) // add threshold
			{
			println("apogee check failed");
			print(apogee_buffer[i]);
			print(", ");
			print(apogee_buffer[i + 1]);
			return EXIT_FAILURE;
			}
		}
	println("apogee check succeeded");
	return EXIT_SUCCESS;
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

void soft_recovery_mode()
	{
	println("[ROCKET STATE] SOFT RECOVERY");

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
	println("[ROCKET STATE] BALLISTIC DESCENT");

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

int debug_data()
	{
#ifdef ROCKET_DEBUGMODE

	String data_string_fmt = "millis(), rocket_state, kx134_accel_x, kx134_accel_y, kx134_accel_z, relative_altitude";
	String data_string     = "";

	if (debug_time == 0UL)
		{
		debug_time = millis();
		}

	if ((millis() - debug_time) < DEBUG_INTERVAL)
		{
		return EXIT_FAILURE;
		}

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

	println(data_string_fmt);
	print("data_string: ");
	println(data_string);

	scan_and_print_I2C_devices();

	debug_time = 0UL;

#endif // ROCKET_DEBUGMODE

	return EXIT_SUCCESS;
	}

// STANDARD ENTRY POINTS
void setup()
	{
	Serial.begin(9600); // arg doesnt need to be 9600 just true
	Wire.begin();

#if (TESTMODE == 1)
	#warning "TESTMODE ENABLED"
	#warning "only tests will be run, no rocket code"
	println("[TESTMODE] testing Teensy 4.1");
	test_main();
#endif // TESTMODE

	init_all();

	if (health_check() == EXIT_FAILURE)
		{
		println("[FAILED] Health Check"); // also write to reserved fram space
		exit(1);                          // this should also fail if init_all() fails;
		}

	// write_to_sd_card(EVENTLOG, "setup exit");

	buzzer_on();

	config.trigger  = WATCHDOG_TRIGGER;
	config.timeout  = WATCHDOG_TIMEOUT;
	config.callback = watchdog_callback;
	wdt.begin(config);
	wdt.feed();

	// TODO: add a method to read previous state from FRAM and restore it
	// int value_from_fram = read_from_fram(0x0);
	// rocket_state = set_state_for_statemachine(&rocket_state, value_from_fram);

	println("setup() exit");
	}

void loop()
	{
	// debug_data();
	wdt.feed();
	// select_flight_mode(rocket_state);
	println(apogee_check());
	}
