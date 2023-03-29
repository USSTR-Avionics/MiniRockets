#if (TESTMODE == 1)

	#include "debug_macros.h"
	#include "sensor_fram.h"
	#include "package_fram.h"
	#include "sensor_led.h"


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
    int upper = 2500;
    // write to FRAM
    for (int i = 0; i < upper; i++)
        {
        write_data_chunk_to_fram(millis(), 2, 0.5f, 0.5f, 9.8f, 0.0f, 90.0f, 180.0f, 100, 45000.0f, 35.5f);
        }

    // read from FRAM
    uint16_t test_cursor = FRAM_INIT_ADDRESS;
    for (int i = 0; i < upper; i++)
        {
        read_data_chunk_from_fram(test_cursor);
        test_cursor += 25;
        println("test_cursor: " +String(test_cursor));
        println("millis: " + String(sensor_chunk.timestamp));
        println("current state: " + String(sensor_chunk.current_state));
        println("accl x: " + String(sensor_chunk.accl_x));
        println("accl y: " + String(sensor_chunk.accl_y));
        println("accl z: " + String(sensor_chunk.accl_z));
        println("gyro x: " + String(sensor_chunk.gyro_x));
        println("gyro y: " + String(sensor_chunk.gyro_y));
        println("gyro z: " + String(sensor_chunk.gyro_z));
        println("rel alt: " + String(sensor_chunk.rel_alt));
        println("pressure: " + String(sensor_chunk.pressure));
        println("temp: " + String(sensor_chunk.thermocouple_temp));
        }
	}

int test_main()
	{
	delay(1000);
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
