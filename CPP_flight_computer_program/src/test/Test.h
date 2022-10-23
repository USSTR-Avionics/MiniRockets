#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H

#include "rusty_fram.h"

enum AVR
{
    AVR_FAILED,
    AVR_SUCCESS,
    AVR_TIME_OUT,
};

template<typename T>
class Test
{
public:
    
    /* spawns new thread for each call
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
    */
    
    enum AVR Test_Value(const std::function<T(T, T)>& Function, const T& Value, const T& Expect) const
    {
        Function(Value, Expect) ? return AVR_SUCCESS : AVR_FAILED;
    }
    
};



#endif //CPP_FLIGHT_COMPUTER_PROGRAM_TEST_H
