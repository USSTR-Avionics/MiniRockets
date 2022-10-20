#include "rusty_fram.h"
#include <cassert>
#include <unity.h>


void wrapper_tests()
    {
    // within range inputs
    TEST_ASSERT_EQUAL(101, wrap_temperature_for_writing(0)); // params: expected, actual
    TEST_ASSERT_EQUAL(1, wrap_temperature_for_writing(-100));
    TEST_ASSERT_EQUAL(201, wrap_temperature_for_writing(100));

    // out of range inputs


    // invalid inputs

    // TODO: Add more tests

    }

void setup() 
    { 
    UNITY_BEGIN();
    RUN_TEST(wrapper_tests);
    UNITY_END();
    }