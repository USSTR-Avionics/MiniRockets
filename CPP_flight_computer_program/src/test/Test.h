#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H

#include <future>
#include <chrono>

#include "rusty_fram.h"

enum AVR
{
    AVR_FAILED,
    AVR_SUCCESS,
    AVR_TIME_OUT,
} AVR;

template<typename T>
class Test
{
public:
    Test();

    // spawns new thread for each call
    enum AVR Test_Value(const T& Value, const T& Expect, int Delay) const
    {
        std::future<T> Result = std::async(std::launch::async, wrap_temperature_for_writing, Value);

        std::future_status Status = Result.wait_for(std::chrono::milliseconds (Delay));

        if (Status == std::future_status::ready)
        {
            return (Expect == Result.get() ? AVR_SUCCESS : AVR_FAILED);
        }
        else
        {
            return AVR_TIME_OUT;
        }
    };

};



#endif //CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H
