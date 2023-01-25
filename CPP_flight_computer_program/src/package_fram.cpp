#include "sensor_fram.h"
#include <Arduino.h>
#include <stdint.h>

<<<<<<< HEAD
#define ERR_READING  0
=======
>>>>>>> master
#define WRITE_OFFSET 0x10
int fram_cursor = 0x10;

int write_temperature_to_fram(float temp)
    {
    // TODO: test and work
<<<<<<< HEAD
    float upper_bound = 100.0f;
    float lower_bound = -100.0f;

    if (temp > upper_bound || temp < lower_bound)
        {
        write_to_fram(fram_cursor, ERR_READING);
        fram_cursor += 1;
        return EXIT_FAILURE;
        }

    uint8_t whole_part = (uint8_t)temp;
    uint8_t decimal_part = (uint8_t)((temp - whole_part) * 1000);
    whole_part += 101;

    write_to_fram(fram_cursor, whole_part);
    fram_cursor += 1;
    write_to_fram(fram_cursor, decimal_part);
    fram_cursor += 1;

=======
    int whole_part = (int)temp;
    int decimal_part = (int)((temp - whole_part) * 1000);

    write_to_fram(fram_cursor, whole_part);
    fram_cursor += 1;
    write_to_fram(fram_cursor, decimal_part);
    fram_cursor += 1;

>>>>>>> master
    return EXIT_SUCCESS;
    }

int write_acceleration_to_fram(float accel)
    {
<<<<<<< HEAD
    // TODO: test and work
    uint8_t whole_part = (uint8_t)accel;
    uint8_t decimal_part = (uint8_t)((accel - whole_part) * 1000);
    whole_part += 101;

    write_to_fram(fram_cursor, whole_part);
    fram_cursor += 1;
    write_to_fram(fram_cursor, decimal_part);
    fram_cursor += 1;

    return EXIT_SUCCESS;
    }
=======
    return EXIT_SUCCESS;
    }
>>>>>>> master
