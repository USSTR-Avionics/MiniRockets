#include "package_fram.h"
#include "debug_macros.h"
#include "half.h"
#include "sensor_fram.h"
#include <Arduino.h>
#include <cstdint>
#include <stdint.h>


// it's okay to ignore this as we're using it for reading only!
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wdouble-promotion"


static uint32_t fram_cursor      = FRAM_INIT_ADDRESS;

// definition of extern global sensor_chunk
sensor_chunk_layout sensor_chunk = {};

int init_fram_package()
	{
	return init_fram();
	}

int write_test_data_to_fram()
	{
	int result = 0;
	result += write_to_fram(28, 0x20);
	result += write_to_fram(29, 0x21);
	result += write_to_fram(30, 0x22);
	result += write_to_fram(31, 0x23);
	result += write_to_fram(32, 0x24);

	if (result == 0)
		{
		return EXIT_SUCCESS;
		}
	return EXIT_FAILURE;
	}

int read_test_data_from_fram()
	{
	int result = 0;
	result += read_from_fram(0x20);
	result += read_from_fram(0x21);
	result += read_from_fram(0x22);
	result += read_from_fram(0x23);
	result += read_from_fram(0x24);

	if (result == 150)
		{
		return EXIT_SUCCESS;
		}
	return EXIT_FAILURE;
	}

void print_current_sensor_chunk(bool printheader)
	{
	if (printheader)
		{
		// print csv format header
		String csv_header = "cursor,timestamp,rocket_state,accl_x,accl_y,accl_z,gyro_x,gyro_y,gyro_z,rel_alt,pressure,thermocouple_temp";
		println(csv_header);
		}

	// construct string for printing
	char print_string[100];
	sprintf(
	    print_string,
	    "%lx,%lu,%u,%f,%f,%f,%f,%f,%f,%f,%f,%f",
	    fram_cursor, sensor_chunk.timestamp, sensor_chunk.current_state,
	    (float)sensor_chunk.accl_x, (float)sensor_chunk.accl_y, (float)sensor_chunk.accl_z,
	    (float)sensor_chunk.gyro_x, (float)sensor_chunk.gyro_y, (float)sensor_chunk.gyro_z,
	    (float)sensor_chunk.rel_alt, (float)sensor_chunk.pressure, (float)sensor_chunk.thermocouple_temp);

	// print string
	println(print_string);
	}

void dump_fram_to_serial()
	{
	fram_cursor = FRAM_INIT_ADDRESS;
	while (true)
		{
		if (read_data_chunk_from_fram(fram_cursor) == EXIT_FAILURE)
			{
			break;
			}
		print_current_sensor_chunk(false);
		fram_cursor += 25;
		}
	}

auto write_float32_to_fram(float data) -> int
	{
	uint8_t* ptr_to_f32 = (uint8_t*)&data;

	write_to_fram(ptr_to_f32[0], fram_cursor);
	fram_cursor++;
	write_to_fram(ptr_to_f32[1], fram_cursor);
	fram_cursor++;
	write_to_fram(ptr_to_f32[2], fram_cursor);
	fram_cursor++;
	write_to_fram(ptr_to_f32[3], fram_cursor);
	fram_cursor++;

	return EXIT_SUCCESS;
	}

auto read_float32_from_fram(int where) -> float
	{
	uint8_t float16_arr[4];

	float16_arr[0] = read_from_fram(where);
	where++;
	float16_arr[1] = read_from_fram(where);
	where++;
	float16_arr[2] = read_from_fram(where);
	where++;
	float16_arr[3] = read_from_fram(where);
	where++;

	float f32_val = *(float*)float16_arr;

	return f32_val;
	}

/**
 * @brief Writes a f16 data type to the FRAM.
 * @post The FRAM cursor will be incremented by 2.
 * @param data The data to write to the FRAM.
 * @return EXIT_SUCCESS if the write succeeded, EXIT_FAILURE otherwise.
 */
auto write_float16_to_fram(float data) -> int
	{
	FLOAT16 val_16      = FLOAT16(data);
	uint8_t* ptr_to_f16 = (uint8_t*)&val_16;

	write_to_fram(ptr_to_f16[0], fram_cursor);
	fram_cursor++;
	write_to_fram(ptr_to_f16[1], fram_cursor);
	fram_cursor++;

	return EXIT_SUCCESS;
	}

auto read_float16_from_fram(int where) -> float
	{
	uint8_t float16_arr[2];

	float16_arr[0] = read_from_fram(where);
	where++;
	float16_arr[1] = read_from_fram(where);
	where++;

	FLOAT16 f16_val = *(FLOAT16*)float16_arr;

	return FLOAT16::ToFloat32(f16_val);
	}

/*
 * @brief Writes a data chunk to the FRAM.
 * @param timestamp The timestamp of the data chunk.
 * @param current_state The current state of the rocket.
 * @param accl_x The x axis acceleration.
 * @param accl_y The y axis acceleration.
 * @param accl_z The z axis acceleration.
 * @param gyro_x The x axis gyroscope.
 * @param gyro_y The y axis gyroscope.
 * @param gyro_z The z axis gyroscope.
 * @param rel_alt The relative altitude.
 * @param pressure The pressure.
 * @param thermocouple_temp The thermocouple temperature.
 * @return EXIT_SUCCESS if the write succeeded, EXIT_FAILURE otherwise.
 */
int write_data_chunk_to_fram(
    uint32_t timestamp, uint8_t current_state,
    float accl_x, float accl_y, float accl_z,
    float gyro_x, float gyro_y, float gyro_z,
    float rel_alt, float pressure, float thermocouple_temp)
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
	// | pressure             | 04 bytes  | float32   |
	// | thermocouple temp    | 02 bytes  | float16   |
	// |----------------------|-----------|-----------|
	// | total                | 25 bytes  |           |
	// |----------------------|-----------|-----------|

	if (fram_cursor + 25 > FRAM_MAX_ADDRESS)
		{
		// error, we're out of space
		return EXIT_FAILURE;
		}

	// write timestamp
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
	write_to_fram(current_state, fram_cursor);
	fram_cursor++;

	// write acceleration x axis
	write_float16_to_fram(accl_x);

	// write acceleration y axis
	write_float16_to_fram(accl_y);

	// write acceleration z axis
	write_float16_to_fram(accl_z);

	// write gyroscope x axis
	write_float16_to_fram(gyro_x);

	// write gyroscope y axis
	write_float16_to_fram(gyro_y);

	// write gyroscope z axis
	write_float16_to_fram(gyro_z);

	// write relative altitude
	write_float16_to_fram(rel_alt);

	// write pressure
	write_float32_to_fram(pressure);

	// write thermocouple temp
	write_float16_to_fram(thermocouple_temp);

	return EXIT_SUCCESS;
	}

int read_data_chunk_from_fram(uint32_t cursor_position)
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
	// | pressure             | 04 bytes  | float32   |
	// | thermocouple temp    | 02 bytes  | float16   |
	// |----------------------|-----------|-----------|
	// | total                | 25 bytes  |           |
	// |----------------------|-----------|-----------|

	if (cursor_position + 25 > FRAM_MAX_ADDRESS)
		{
		// error, we're out of space
		return EXIT_FAILURE;
		}

	// read timestamp
	uint32_t timestamp  = 0;
	uint8_t* ptr_to_u32 = (uint8_t*)&timestamp;
	ptr_to_u32[0]       = read_from_fram(cursor_position);
	cursor_position++;
	ptr_to_u32[1] = read_from_fram(cursor_position);
	cursor_position++;
	ptr_to_u32[2] = read_from_fram(cursor_position);
	cursor_position++;
	ptr_to_u32[3] = read_from_fram(cursor_position);
	cursor_position++;

	sensor_chunk.timestamp = timestamp;

	// read rocket state
	uint8_t current_state  = read_from_fram(cursor_position);
	cursor_position++;

	println("current state: " + String(current_state));

	sensor_chunk.current_state = current_state;

	// read acceleration x axis
	float accl_x               = read_float16_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;

	sensor_chunk.accl_x = accl_x;

	// read acceleration y axis
	float accl_y        = read_float16_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;

	sensor_chunk.accl_y = accl_y;

	// read acceleration z axis
	float accl_z        = read_float16_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;

	sensor_chunk.accl_z = accl_z;

	// read gyroscope x axis
	float gyro_x        = read_float16_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;

	sensor_chunk.gyro_x = gyro_x;

	// read gyroscope y axis
	float gyro_y        = read_float16_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;

	sensor_chunk.gyro_y = gyro_y;

	// read gyroscope z axis
	float gyro_z        = read_float16_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;

	sensor_chunk.gyro_z = gyro_z;

	// read relative altitude
	float rel_alt       = read_float16_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;

	sensor_chunk.rel_alt = rel_alt;

	// read pressure
	float pressure       = read_float32_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;
	cursor_position++;
	cursor_position++;

	sensor_chunk.pressure   = pressure;

	// read thermocouple temp
	float thermocouple_temp = read_float16_from_fram(cursor_position);
	cursor_position++;
	cursor_position++;

	sensor_chunk.thermocouple_temp = thermocouple_temp;

	return EXIT_SUCCESS;
	}
