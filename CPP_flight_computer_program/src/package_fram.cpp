#include "sensor_fram.h"
#include <Arduino.h>

#define WRITE_OFFSET 0x10
int fram_cursor = 0x10;

int write_temperature_to_fram(float temp)
    {
    // TODO: test and work
    int whole_part = (int)temp;
    int decimal_part = (int)((temp - whole_part) * 1000);

    write_to_fram(fram_cursor, whole_part);
    fram_cursor += 1;
    write_to_fram(fram_cursor, decimal_part);
    fram_cursor += 1;

    return EXIT_SUCCESS;
    }

int write_acceleration_to_fram(float accel)
    {
    return EXIT_SUCCESS;
    }