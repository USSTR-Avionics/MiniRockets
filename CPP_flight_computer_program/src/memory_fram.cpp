#include "Adafruit_FRAM_I2C.h"
#include "rusty_fram.h"
#include <Arduino.h>

Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();

#define WRITE_OFFSET 0x10

int last_write_temp = 0x10;
int last_write_accl = 0x12;

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
    return EXIT_FAILURE;
    }

int write_temperature_to_fram(float t)
    {
    uint8_t* pointerToIndex0 = wrap_temperature_for_writing(t); // [u8; 2]
    write_to_fram(pointerToIndex0[0], last_write_temp + WRITE_OFFSET);
    write_to_fram(pointerToIndex0[1], last_write_temp + 1 + WRITE_OFFSET);
    last_write_temp += 2;
    }

int write_acceleration_to_fram(float a)
    {
    uint8_t* pointerToIndex0 = wrap_acceleration_for_writing(a); // [u8; 4]
    write_to_fram(pointerToIndex0[0], last_write_accl + WRITE_OFFSET);
    write_to_fram(pointerToIndex0[1], last_write_accl + 1 + WRITE_OFFSET);
    write_to_fram(pointerToIndex0[2], last_write_accl + 2 + WRITE_OFFSET);
    write_to_fram(pointerToIndex0[3], last_write_accl + 3 + WRITE_OFFSET);
    last_write_accl += 4;
    }
    

int c_return_delay_test()
    {
    while (true)
        {
        }
    return 0;
    }