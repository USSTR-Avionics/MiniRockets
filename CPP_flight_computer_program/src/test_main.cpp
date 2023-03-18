#if (TESTMODE == 1)

	#include "debug_macros.h"
	#include "sensor_fram.h"
	#include "package_fram.h"
	#include "unity.h"


void setup_test_sensor_fram()
	{
    init_fram_package();    
	}

void teardown_test_sensor_fram()
	{
	// TODO: restore fram to original state
	}

void test_sensor_fram()
	{
    write_data_chunk_to_fram(1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
    read_data_chunk_from_fram(FRAM_INIT_ADDRESS);

    println(sensor_chunk.timestamp);
    println(sensor_chunk.current_state);
    println(sensor_chunk.accl_x);
    println(sensor_chunk.accl_y);
    println(sensor_chunk.accl_z);
    println(sensor_chunk.gyro_x);
    println(sensor_chunk.gyro_y);
    println(sensor_chunk.gyro_z);
    println(sensor_chunk.rel_alt);
    println(sensor_chunk.pressure);
    println(sensor_chunk.thermocouple_temp);

    for (int i = 1; i <= 100; i++)
    {
        
        write_data_chunk_to_fram(millis(), 6 + i, 0.12f + i / 10, 0.24f, 9.8f, 0.0f, 45.5f, 90.0f, 123.456f, 45000.75, 223.56f);

    }

    for (int a = 1; a <= 100; a++)
    {
        read_data_chunk_from_fram(FRAM_INIT_ADDRESS + 25 * a);

        println(sensor_chunk.timestamp);
        println(sensor_chunk.current_state);
        println(sensor_chunk.accl_x);
        println(sensor_chunk.accl_y);
        println(sensor_chunk.accl_z);
        println(sensor_chunk.gyro_x);
        println(sensor_chunk.gyro_y);
        println(sensor_chunk.gyro_z);
        println(sensor_chunk.rel_alt);
        println(sensor_chunk.pressure);
        println(sensor_chunk.thermocouple_temp);
    }

    println("finished");

	}

int test_main()
	{
	println("[TESTMODE] tests starting");

	println("testing fram...");
	setup_test_sensor_fram();
	test_sensor_fram();
	teardown_test_sensor_fram();
	println("testing fram...done");

	println("[TESTMODE] tests completed");
	exit(1);
	}

void setUp(void) // run before each test
	{
	}

void tearDown(void) // run after each test
	{
	}

#endif // TESTMODE
