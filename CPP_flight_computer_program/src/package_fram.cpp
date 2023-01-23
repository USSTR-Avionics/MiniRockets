#include "Adafruit_FRAM_I2C.h"
#include <Arduino.h>

Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();

#define WRITE_OFFSET 0x10
#define FRAM_I2C_ADDRESS 0x50

int fram_cursor = 0x10;

int init_fram()
    {
    Serial.println("init_fram()");
    fram.begin(FRAM_I2C_ADDRESS);

    return EXIT_SUCCESS;
    }

int write_to_fram(int what, int where)
    {
    return EXIT_FAILURE;
    }

int write_temperature_to_fram(float temp)
    {
    // TODO: test and work
    int whole_part = (int)temp;
    int decimal_part = (int)((temp - whole_part) * 1000);

    fram.write(fram_cursor, whole_part);
    fram_cursor += 1;
    fram.write(fram_cursor, decimal_part);
    fram_cursor += 1;

    return EXIT_SUCCESS;
    }

int write_acceleration_to_fram(float accel)
    {
    return EXIT_SUCCESS;
    }