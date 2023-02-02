#if (TESTMODE == 1)

	#include "debug_macros.h"
	#include "sensor_fram.h"
	#include "unity.h"


void test_fram_read_write()
	{
	init_fram();

	uint8_t data = 1;

	write_to_fram(data, 0x10);
	uint8_t result = read_from_fram(0x10);

	println(data);
	println(result);

	write_to_fram(10, 0);
	write_to_fram(0, 10);
	println(read_from_fram(0));
	}

int test_main()
	{
	println("[TESTMODE] tests starting");
	test_fram_read_write();
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
