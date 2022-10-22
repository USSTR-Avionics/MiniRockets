#include "Adafruit_FRAM_I2C.h"

Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();

int init_fram()
    {
    Serial.println("init_fram()");
    fram.begin(0x50);
    fram.write(0x0, 28);
    Serial.println("read from fram");
    Serial.println(fram.read(0x0));

    return EXIT_SUCCESS;
    }

int write_to_fram(int what, int where)
    {

    }