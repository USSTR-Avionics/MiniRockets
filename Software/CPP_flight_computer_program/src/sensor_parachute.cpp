#include "sensor_pins.h"
#include <Arduino.h>


/**
 * @brief    deploy the parachute
 * @post     the parachute is deployed
*/
void deploy_parachute()
	{
    pinMode(PARACHUTE_PIN, OUTPUT);
    digitalWrite(PARACHUTE_PIN, HIGH);
	}