#include "debug_macros.h"
#include "sensor_fram.h"
#include "half.h"
#include <Arduino.h>
#include <cstdint>
#include <stdint.h>


// it's okay to ignore this as we're using it for reading only!
#pragma GCC diagnostic ignored "-Wstrict-aliasing" 


int init_fram_package()
    {
    return init_fram();
    }

void dump_fram_to_serial()
	{
	// TODO: build csv string and dump to USB serial
	}

/**
 * @brief Writes a f16 data type to the FRAM.
 * @post The FRAM cursor will be incremented by 2.
 * @param data The data to write to the FRAM.
 * @return EXIT_SUCCESS if the write succeeded, EXIT_FAILURE otherwise.
 */
auto write_float16_to_fram(float data) -> int
	{
    FLOAT16 val_16 = FLOAT16(data);
    uint8_t* ptr_to_f16 = (uint8_t*)&val_16;

    write_to_fram(ptr_to_f16[0], fram_cursor);
    fram_cursor++;
    write_to_fram(ptr_to_f16[1], fram_cursor);

	return EXIT_SUCCESS;
	}

auto read_float16_from_fram(int where) -> float
	{
    uint8_t float16_arr[2];

    float16_arr[0] = read_from_fram(where);
    where++;
    float16_arr[1] = read_from_fram(where);

    FLOAT16 f16_val = *(FLOAT16*)float16_arr;

    return FLOAT16::ToFloat32(f16_val);
	}

int write_data_chunk_to_fram(
    uint32_t timestamp, uint8_t current_state, 
    float accl_x, float accl_y, float accl_z, 
    float gyro_x, float gyro_y, float gyro_z,
    float rel_alt, float pressure, float thermocouple_temp,
    uint16_t extra_space)
    {
    // data chunk format
    // |----------------------|-----------|-----------|
    // |      data            |   size    | data type |
    // |----------------------|-----------|-----------|
    // | timestamp            | 04 bytes  | u32       |
    // | rocket state         | 01 byte   | u8        |
    // | acceleration x axis  | 02 bytes  | float16   |
    // | acceleration y axis  | 02 bytes  | float16   |
    // | acceleration z axis  | 02 bytes  | float16   |
    // | gyroscope x axis     | 02 bytes  | float16   |
    // | gyroscope y axis     | 02 bytes  | float16   |
    // | gyroscope z axis     | 02 bytes  | float16   |
    // | relative altitude    | 02 bytes  | float16   |
    // | pressure             | 02 bytes  | float16   |
    // | thermocouple temp    | 02 bytes  | float16   |
    // | extra space          | 02 byte   | undefined |
    // |----------------------|-----------|-----------|
    // | total                | 25 bytes  |           |
    // |----------------------|-----------|-----------|

    println("--------------------------------------------------fram cursor before write: " + String(fram_cursor));

    if (fram_cursor + 25 > FRAM_MAX_ADDRESS)
        {
        // error, we're out of space
        return EXIT_FAILURE;
        }

    // write timestamp
    println("Writing timestamp: " + String(timestamp) + " to FRAM at address: " + String(fram_cursor));

    uint8_t* ptr_to_u32 = (uint8_t*)&timestamp;
    write_to_fram(ptr_to_u32[0], fram_cursor);
    fram_cursor++;
    write_to_fram(ptr_to_u32[1], fram_cursor);
    fram_cursor++;
    write_to_fram(ptr_to_u32[2], fram_cursor);
    fram_cursor++;
    write_to_fram(ptr_to_u32[3], fram_cursor);
    fram_cursor++;

    // write rocket state
    println("Writing rocket state: " + String(current_state) + " to FRAM at address: " + String(fram_cursor));

    write_to_fram(current_state, fram_cursor);
    fram_cursor++;

    // write acceleration x axis
    println("Writing acceleration x axis: " + String(accl_x) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(accl_x);

    // write acceleration y axis
    println("Writing acceleration y axis: " + String(accl_y) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(accl_y);

    // write acceleration z axis
    println("Writing acceleration z axis: " + String(accl_z) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(accl_z);

    // write gyroscope x axis
    println("Writing gyroscope x axis: " + String(gyro_x) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(gyro_x);

    // write gyroscope y axis
    println("Writing gyroscope y axis: " + String(gyro_y) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(gyro_y);

    // write gyroscope z axis
    println("Writing gyroscope z axis: " + String(gyro_z) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(gyro_z);

    // write relative altitude
    println("Writing relative altitude: " + String(rel_alt) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(rel_alt);

    // write pressure
    println("Writing pressure: " + String(pressure) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(pressure);

    // write thermocouple temp
    println("Writing thermocouple temp: " + String(thermocouple_temp) + " to FRAM at address: " + fram_cursor);
    write_float16_to_fram(thermocouple_temp);

    // write extra space
    // ! unimplemented()
    println("Writing extra space: " + String(extra_space) + " to FRAM at address: " + fram_cursor);
    extra_space = 0;
    fram_cursor++;
    fram_cursor++;

    println("----------------------------------------------------fram cursor after write: " + String(fram_cursor));

    return EXIT_SUCCESS;
    }

void read_data_chunk_from_fram(uint8_t cursor_position)
    {
    // data chunk format
    // |----------------------|-----------|-----------|
    // |      data            |   size    | data type |
    // |----------------------|-----------|-----------|
    // | timestamp            | 04 bytes  | u32       |
    // | rocket state         | 01 byte   | u8        |
    // | acceleration x axis  | 02 bytes  | float16   |
    // | acceleration y axis  | 02 bytes  | float16   |
    // | acceleration z axis  | 02 bytes  | float16   |
    // | gyroscope x axis     | 02 bytes  | float16   |
    // | gyroscope y axis     | 02 bytes  | float16   |
    // | gyroscope z axis     | 02 bytes  | float16   |
    // | relative altitude    | 02 bytes  | float16   |
    // | pressure             | 02 bytes  | float16   |
    // | thermocouple temp    | 02 bytes  | float16   |
    // | extra space          | 02 byte   | undefined |
    // |----------------------|-----------|-----------|
    // | total                | 25 bytes  |           |

    println("------------------------------------------------------fram cursor before read: " + String(cursor_position));

    // read timestamp
    uint32_t timestamp = 0;
    uint8_t* ptr_to_u32 = (uint8_t*)&timestamp;
    ptr_to_u32[0] = read_from_fram(cursor_position);
    cursor_position++;
    ptr_to_u32[1] = read_from_fram(cursor_position);
    cursor_position++;
    ptr_to_u32[2] = read_from_fram(cursor_position);
    cursor_position++;
    ptr_to_u32[3] = read_from_fram(cursor_position);
    cursor_position++;

    println("read timestamp at cursor position " + String(cursor_position - 4) + ": " + String(timestamp));

    // read rocket state
    uint8_t current_state = read_from_fram(cursor_position);
    cursor_position++;

    println("read rocket state at cursor position " + String(cursor_position - 1) + ": " + String(current_state));

    // read acceleration x axis
    float accl_x = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read acceleration x axis at cursor position " + String(cursor_position - 2) + ": " + String(accl_x));

    // read acceleration y axis
    float accl_y = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read acceleration y axis at cursor position " + String(cursor_position - 2) + ": " + String(accl_y));

    // read acceleration z axis
    float accl_z = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read acceleration z axis at cursor position " + String(cursor_position - 2) + ": " + String(accl_z));

    // read gyroscope x axis
    float gyro_x = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read gyroscope x axis at cursor position " + String(cursor_position - 2) + ": " + String(gyro_x));

    // read gyroscope y axis
    float gyro_y = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read gyroscope y axis at cursor position " + String(cursor_position - 2) + ": " + String(gyro_y));

    // read gyroscope z axis
    float gyro_z = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read gyroscope z axis at cursor position " + String(cursor_position - 2) + ": " + String(gyro_z));

    // read relative altitude
    float rel_alt = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read relative altitude at cursor position " + String(cursor_position - 2) + ": " + String(rel_alt));

    // read pressure
    float pressure = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read pressure at cursor position " + String(cursor_position - 2) + ": " + String(pressure));

    // read thermocouple temp
    float thermocouple_temp = read_float16_from_fram(cursor_position);
    cursor_position++;
    cursor_position++;

    println("read thermocouple temp at cursor position " + String(cursor_position - 2) + ": " + String(thermocouple_temp));

    // read extra space
    // ! unimplemented()
    cursor_position++;
    cursor_position++;

    int extra_space = 0;
    println("read extra space at cursor position " + String(cursor_position - 2) + ": " + String(extra_space));

    println("----------------------------------------------------------fram cursor after read: " + String(cursor_position));

    // print csv format header
    String csv_header = "timestamp,rocket_state,accl_x,accl_y,accl_z,gyro_x,gyro_y,gyro_z,rel_alt,pressure,thermocouple_temp";
    println(csv_header);

    // construct string for printing
    char print_string[100];
    sprintf(print_string, "%lu,%u,%f,%f,%f,%f,%f,%f,%f,%f,%f", timestamp, current_state, accl_x, accl_y, accl_z, gyro_x, gyro_y, gyro_z, rel_alt, pressure, thermocouple_temp);

    // print string
    println(print_string);
    }