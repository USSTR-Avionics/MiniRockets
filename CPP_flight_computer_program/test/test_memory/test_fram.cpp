#include "rusty_fram.h"
#include <Arduino.h>
#include <unity.h>


void test_wrapper()
    {
    // within range inputs
    TEST_ASSERT_EQUAL_UINT8(101, wrap_temperature_for_writing(0)); // params: expected, actual
    TEST_ASSERT_EQUAL_UINT8(1, wrap_temperature_for_writing(-100));
    TEST_ASSERT_EQUAL_UINT8(201, wrap_temperature_for_writing(100));

    // out of range inputs


    // invalid inputs

    // TODO: Add more tests

    }

void setUp() {
  // set stuff up here
}

void tearDown() {
  // clean stuff up here
}

void setup() 
    { 
    Serial.begin(9600);
    delay(500);
    UNITY_BEGIN();
    RUN_TEST(test_wrapper);
    UNITY_END();
    }

int main() {return 1;}