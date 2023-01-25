#include "Adafruit_FRAM_I2C.h"
#include "package_fram.h"
#include <Arduino.h>

#define FRAM_I2C_ADDRESS 0x50

Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();


int init_fram()
    {
    Serial.println("init_fram()");
    fram.begin(FRAM_I2C_ADDRESS);

    return EXIT_SUCCESS;
    }

int write_to_fram(int what, int where)
    {
    fram.write(where, what);
    return EXIT_SUCCESS;
    }