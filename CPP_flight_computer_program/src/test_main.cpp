#if (TESTMODE == 1)

#include <unity.h>

#include "debug_macros.h"

#include "sensor_fram.h"


void test_fram_read_write()
    {
    init_fram();

    write_to_fram(1, 0x10);
    TEST_ASSERT_EQUAL_INT(1, read_from_fram(0x10));
    }

void test_main()
    {
    println("[TESTMODE] tests starting");
    UNITY_BEGIN();
    RUN_TEST(test_fram_read_write);
    UNITY_END();
    println("[TESTMODE] tests completed");
    }

void setUp(void) // run before each test
    {
    }

void tearDown(void) // run after each test
    {
    }

#endif // TESTMODE