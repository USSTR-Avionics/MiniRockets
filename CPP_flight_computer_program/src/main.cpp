#include "package_fram.h"
#include "package_statemachine_t.h"
#include "package_statistics.h"
#include "package_testmode.h"
#include "package_watchdog.h"

#include "sensor_bmp280.h"
#include "sensor_buzzer.h"
#include "sensor_kx134.h"
#include "sensor_led.h"
#include "sensor_parachute.h"
#include "sensor_radio.h"
#include "sensor_thermocouple.h"
#include <RH_RF95.h>

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
unsigned long write_time    = 0UL;
int descent_check           = 0;
float last_alt              = 0;


// STATE MACHINE
static statemachine_t::e_rocket_state rocket_state;

// RFM95 radio, hard code the pins once board is developed
/*
YOU CAN ONLY HAVE 2 INSTANCES OF THIS OBJ AT 1 TIME (3 IF MEGA)
    LoRa packet format:
        8 symbol PREAMBLE
        Explicit header with header CRC (default CCITT, handled internally by the radio)
        4 octets HEADER: (TO, FROM, ID, FLAGS)
        0 to 251 octets DATA
        CRC (default CCITT, handled internally by the radio)

     Settings:
        It is very important therefore, that if you are using the RH_RF95 driver with another SPI based device,
        that you disable interrupts while you transfer data o and from that other device.
        Use cli() to disable interrupts and sei() to readable them.
*/
const uint16_t RF95_CS { 10 };
const uint16_t RF95_int { 32 };
static RH_RF95 RF95(RF95_CS, RF95_int);
const uint32_t RF95_Max_message_length = RF95.maxMessageLength();

// GLOBAL VARS
float ground_base_pressure             = 0.0f;
float ground_base_altitude             = 0.0f;
float rocket_altitude                  = 0.0f;

float kx134_accel_x                    = 0.0f;
float kx134_accel_y                    = 0.0f;
float kx134_accel_z                    = 0.0f;

float rocket_apogee                    = 0.0f;



int init_all()
	{
	init_kx134();
	init_bmp280();
	init_fram_package();
	init_LED();

	// TODO:
	// init_bmi088();
	// init_RFM95_TX();

	// Setting global variables
	ground_base_pressure = get_bmp280_pressure();
	ground_base_altitude = get_bmp280_altitude(ground_base_pressure);
	rocket_state         = statemachine_t::e_rocket_state::unarmed;

	// zombie pin init
	pinMode(PIN_A16, INPUT_PULLUP);

#if ROCKET_DEBUGMODE == 1
	rocket_state = set_current_state_for_statemachine(rocket_state, GROUND_IDLE_STATE);
#endif

	return EXIT_SUCCESS;
	}

int health_check()
	{
	println("running health_check()");

	// KX134 checks
	float z_thresh_high = 11.0;
	float z_thresh_low  = 9.0;

	int count           = 0;
	while (count < 10)
		{
		float curr_z_reading = get_kx134_accel_z();
		if (curr_z_reading < z_thresh_high && curr_z_reading > z_thresh_low)
			{
			count++;
			}
		else
			{
			println("KX134 health check failed");
			println("curr_z_reading: " + String(curr_z_reading));
			println("z_thresh_low: " + String(z_thresh_low));
			println("z_thresh_high: " + String(z_thresh_high));
			return EXIT_FAILURE;
			}
		}

	// BMP280 checks
	float alt_thresh_low  = -0.25;
	float alt_thresh_high = 0.50;

	count                 = 0;
	while (count < 10)
		{
		float curr_alt_reading = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
		if (curr_alt_reading > alt_thresh_low && curr_alt_reading < alt_thresh_high)
			{
			count++;
			}
		else
			{
			println("BMP280 health check failed");
			println("curr_alt_reading: " + String(curr_alt_reading));
			println("alt_thresh_low: " + String(alt_thresh_low));
			println("alt_thresh_high: " + String(alt_thresh_high));
			return EXIT_FAILURE;
			}
		}

	// FRAM checks
	int fram_write_result = write_test_data_to_fram();
	int fram_read_result  = read_test_data_from_fram();

	if (fram_write_result == EXIT_FAILURE || fram_read_result == EXIT_FAILURE)
		{
		println("FRAM health check failed");
		return EXIT_FAILURE;
		}

	return EXIT_SUCCESS;
	}

void ground_idle_mode()
	{
	// println("[ROCKET STATE] GROUND IDLE");

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
	// println("[ROCKET STATE] POWERED FLIGHT");

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
	// fill buffer with altitude readings taken every APOGEE_READING_INTERVAL milliseconds
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

	// calculate exponential moving average of apogee buffer
	float apogee_buffer_ema = apogee_buffer[0];
	for (int i = 1; i < APOGEE_BUFFER_SIZE; i++)
		{
		apogee_buffer_ema = get_exponential_moving_average(apogee_buffer[i], apogee_buffer_ema, MODERATE_EMA_SMOOTHING);
		}

	bool ema_lessthan_oldestreading = apogee_buffer_ema < apogee_buffer[0];
	bool ema_greaterthan_zero       = apogee_buffer_ema > ZERO_FLOAT;
	bool ema_greaterthan_threshold  = (apogee_buffer[0] - apogee_buffer[APOGEE_BUFFER_SIZE - 1]) > APOGEE_DIFFERENCE_THRESHOLD;

	if (ema_lessthan_oldestreading && ema_greaterthan_zero && ema_greaterthan_threshold)
		{
		return EXIT_SUCCESS;
		}
	else
		{
		return EXIT_FAILURE;
		}
	}

void unpowered_flight_mode()
	{
	// println("[ROCKET STATE] UNPOWERED FLIGHT");

	setLedBlue();

	if (apogee_check() == EXIT_SUCCESS)
		{
		rocket_state = statemachine_t::e_rocket_state::ballistic_descent;
		}
	}

void soft_recovery_mode()
	{
	// println("[ROCKET STATE] SOFT RECOVERY");

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
	// println("[ROCKET STATE] BALLISTIC DESCENT");

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
	// println("[ROCKET STATE] CHUTE DESCENT");

	// TODO: check gyroscope stabilisation over time
	rocket_altitude = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);

	// TODO: Check for acceleration in the z direction that is stabilised
	if (rocket_altitude < LANDING_ALTITUDE)
		{
		rocket_state = statemachine_t::e_rocket_state::land_safe;
		}
	}

void land_safe_mode()
	{
	// println("[ROCKET STATE] LAND SAFE");
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
	rocket_state = statemachine_t::e_rocket_state::soft_recovery;
	loop();
	}

int check_zombie_mode()
	{
	if (digitalRead(PIN_A16) == HIGH)
		{
		return EXIT_SUCCESS;
		}
	return EXIT_FAILURE;
	}

void save_data()
	{
	if (write_time == 0)
		{
		write_time = millis();
		}
	else if (millis() - write_time > WRITE_INTERVAL)
		{
		write_time       = millis();
		float notanumber = std::numeric_limits<float>::quiet_NaN();
		write_data_chunk_to_fram(
		    millis(), rocket_state,
		    kx134_accel_x, kx134_accel_y, kx134_accel_z,
		    notanumber, notanumber, notanumber,
		    rocket_altitude, get_bmp280_pressure(), get_thermocouple_external_temperature());
		}
	}

int debug_data()
	{
#ifdef ROCKET_DEBUGMODE

	String data_string = "";

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

	kx134_accel_x          = get_kx134_accel_x();
	kx134_accel_y          = get_kx134_accel_y();
	kx134_accel_z          = get_kx134_accel_z();
	data_string            = data_string + String(kx134_accel_x) + ",";
	data_string            = data_string + String(kx134_accel_y) + ",";
	data_string            = data_string + String(kx134_accel_z) + ",";

	rocket_altitude        = get_bmp280_relative_altitude(ground_base_pressure, ground_base_altitude);
	data_string            = data_string + String(rocket_altitude);

	String data_string_fmt = "millis(), rocket_state, kx134_accel_x, kx134_accel_y, kx134_accel_z, relative_altitude";
	println(data_string);

	debug_time = 0UL;

#endif // ROCKET_DEBUGMODE

	save_data();

	return EXIT_SUCCESS;
	}

void UDP_Send(const char* Data, const uint16_t& Timeout)
	{
	RF95.send(reinterpret_cast<const uint8_t*>(Data), strlen(Data) + 1);
	RF95.waitPacketSent(Timeout);
	}

const char* UDP_Receive(const uint16_t& Timeout)
	{
	RF95.waitAvailableTimeout(Timeout);
	uint8_t Buffer[RF95_Max_message_length];
	uint8_t Length = sizeof(Buffer);

	return RF95.recv(Buffer, &Length) ? reinterpret_cast<const char*>(Buffer) : "";
	}

void RF95_Set_modem_config(const uint16_t& Index)
	{
	/*
	LoRa Chirp Options:
	typedef enum
	{
	    Bw125Cr45Sf128 = 0,	   //< Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
	    Bw500Cr45Sf128,	           //< Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
	    Bw31_25Cr48Sf512,	   //< Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
	    Bw125Cr48Sf4096,           //< Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range
	} ModemConfigChoice;
	*/
	switch (Index)
		{
	case 0:
		// this is default already
		RF95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
		break;
	case 1:
		RF95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
		break;
	case 2:
		RF95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
		break;
	case 3:
		RF95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
		break;
	default:
		RF95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
		break;
		}
	}

// STANDARD ENTRY POINTS
void setup()
	{
	Serial.begin(9600);
	Wire.begin();

	// ============= NO TOUCH =============
	const uint16_t RF95_reset { 30 };
	pinMode(RF95_reset, OUTPUT);
	digitalWrite(RF95_reset, HIGH);

	digitalWrite(RF95_reset, LOW);
	delay(10);
	digitalWrite(RF95_reset, HIGH);
	delay(10);

	RF95.init();
	// ============= NO TOUCH END =============

	// do the reset for the radio
	RF95.setFrequency(915.7);
	RF95.setTxPower(23);
	RF95_Set_modem_config(2);


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
	delay(1000);
	if (check_zombie_mode() == EXIT_SUCCESS)
		{
		setLedBlue();
		println("[ZOMBIE MODE] detected");
		dump_fram_to_serial();
		exit(0);
		}

	if (health_check() == EXIT_FAILURE)
		{
		setLedRed();
		println("[FAILED] Health Check"); // also write to reserved fram space
		exit(1);                          // this should also fail if init_all() fails;
		}

	config.trigger  = WATCHDOG_TRIGGER;
	config.timeout  = WATCHDOG_TIMEOUT;
	config.callback = watchdog_callback;
	wdt.begin(config);
	wdt.feed();

	// TODO: add a method to read previous state from EEPROM and restore it
	}

void loop()
	{
	setLedGreen();
	wdt.feed();
	debug_data();
	select_flight_mode(rocket_state);

	UDP_Send("abcdefg", 500);
	}
