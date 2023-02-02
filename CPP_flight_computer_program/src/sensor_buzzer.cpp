#include "sensor_buzzer.h"
#include "sensor_pins.h"

#include <Arduino.h>

void buzzer_on()
	{
	int melody[] = { NOTE_CS5, NOTE_DS5, NOTE_FS5 };
	int duration = 500;
	// We only use melody[1] here to not have too long of a delay
	// pin8 output the voice, every scale is 0.5 sencond
	tone(BUZZER_PIN, melody[1], duration);
	}