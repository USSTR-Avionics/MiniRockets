#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H

#include <future>
#include <chrono>
#include <thread>

#include "rusty_fram.h"
#include "memory_fram.h"

// enum result_enum_avr_test
// {
//     AVR_SUCCESS,
//     AVR_FAILED,
//     AVR_TIME_OUT,
// } AVR_return_enum_value;

int test1(int a)
{
    return a;
}

template<typename T>
class Test
{
public:
    Test();

    // spawns new thread for each call
    int Test_Value(const T& Value, const T& Expect, int Delay) const
    {
        std::future<T> Result = std::async(std::launch::async, test1, Value);

        std::future_status Status = Result.wait_for(std::chrono::milliseconds (Delay));

        if (Status == std::future_status::ready)
        {
            return (Expect == Result.get() ? 0 : 1);
        }
        else
        {
            return 2;
        }


    };

};


#endif //CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H
