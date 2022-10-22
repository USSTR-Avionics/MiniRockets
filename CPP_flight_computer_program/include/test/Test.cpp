#include "Test.h"
#include "rusty_fram.h"

template<typename T>
enum AVR Test<T>::Test_Value(const T& Value, const T& Expect, int Delay) const
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
