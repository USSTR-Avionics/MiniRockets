#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H

#include <future>
#include <chrono>

#include "rusty_fram.h"

enum AVR
{
    AVR_SUCCESS,
    AVR_FAILED,
    AVR_TIME_OUT,
} AVR;

template<typename T>
class Test
{
public:
    Test();

    // spawns new thread for each call
    enum AVR Test_Value(const T& Value, const T& Expect, int Delay) const;


private:

};



#endif //CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H
