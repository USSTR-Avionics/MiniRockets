#include "sensor_pins.h"
#include <Arduino.h>

#define BUILTIN_LED 13 // this does not really change unless microcontroller changes


int init_LED()
	{
	pinMode(BUILTIN_LED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	return EXIT_SUCCESS;
	}

void setLedGreen()
	{
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_BLUE, LOW);
	}

void setLedRed()
	{
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, LOW);
	}

void setLedBlue()
	{
	digitalWrite(LED_BLUE, HIGH);
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_GREEN, LOW);
	}

void flashSOS()
	{
	while (true)
		{
		digitalWrite(LED_BLUE, HIGH);
		delay(500);
		digitalWrite(LED_BLUE, LOW);
		}
	}

void flashInternalLed(bool x)
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