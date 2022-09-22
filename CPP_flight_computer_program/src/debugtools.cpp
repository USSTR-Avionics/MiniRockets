#include <Arduino.h>

void console_debug(bool debug, char err[])
    {
    if (debug == true)
        {
        Serial.println(err);
        }
    }