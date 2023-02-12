#include <SPI.h>
#include "Adafruit_MAX31855.h" 
#include "sensor_thermocouple.h"

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

//TODO?
void init_thermocouple()
    {
    }

//RETRUN EXTERNAL TEMPERATURE (READ AT END OF WIRE)
float get_external_temperature()
{
   float external_temp =  thermocouple.readCelsius();
   return external_temp;
}

//RETURN INTERNAL TEMPERATURE (READ ON CHIP)
float get_internal_temperature()
{
    float internal_temp = thermocouple.readInternal();
    return internal_temp;
}