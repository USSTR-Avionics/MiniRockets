#include <Arduino.h>
#include "sensor_kx134.h"

void console_debug(bool debug, char err[])
    {
    if (debug == true)
        {
        Serial.println(err);
        }
    }

void kx134_debug()
    {
    double kx134_accel_x = get_kx134_accel_x();
    double kx134_accel_y = get_kx134_accel_y();
    double kx134_accel_z = get_kx134_accel_z();
    Serial.println(kx134_accel_x);
    Serial.println(kx134_accel_y);
    Serial.println(kx134_accel_z);
    }