#include "rusty_fram.h"
#include <cassert>


void test_fram()
    {
    // within range
    int32_t input1 = 0;
    uint8_t output1 = wrap_temperature_for_writing(input1);
    uint8_t expected1 = 101;
    assert((output1 == expected1));

    int32_t input2 = -100;
    uint8_t output2 = wrap_temperature_for_writing(input2);
    uint8_t expected2 = 1;
    assert((output2 == expected2));

    // TODO: Add more tests
    }



int main()
    {
    // panics on test fail
    test_fram();
    return 0;
    }

void setup() { main(); }
void loop()  {}