
#include "sensor_pins.h"
#include <Arduino.h>

#define BUILTIN_LED 13 // this does not really change unless microcontroller changes


/**
 * @brief  initialize the LED pins
 */
int init_LED()
	{
	pinMode(BUILTIN_LED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	return EXIT_SUCCESS;
	}

/**
 * @brief  resets the led to no colour
*/
void reset_led()
	{
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, LOW);
	}

/**
 * @brief  set the green LED on
 */
void set_led_green()
	{
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_BLUE, LOW);
	}

/**
 * @brief  set the red LED on
 */
void set_led_red()
	{
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, LOW);
	}

/**
 * @brief  set the blue LED on
 */
void set_led_blue()
	{
	digitalWrite(LED_BLUE, HIGH);
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_GREEN, LOW);
	}

/**
 * @brief  flash the internal LED
 */
void flash_internal_led(bool x)
	{
	if (x == true)
		{
		digitalWrite(BUILTIN_LED, HIGH);
		}
	else
		{
		digitalWrite(BUILTIN_LED, LOW);
		}
	}