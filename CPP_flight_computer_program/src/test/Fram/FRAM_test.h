#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H

#include "Adafruit_EEPROM_I2C.h"
#include "Adafruit_FRAM_I2C.h"

// test

class Floating_point
{
public:

    explicit Floating_point(float value) : Value(value) {};

    void operator = (const float& Value);

    Floating_point& operator = (const Floating_point& Other);

    void Write(float Value, int Size);

    float Read();

private:
    Adafruit_FRAM_I2C FRAM = Adafruit_FRAM_I2C();
    float Value;
};
class f16_FRAM : private Floating_point
{

    ~f16_FRAM();
};

class f32_FRAM : private Floating_point
{

    ~f32_FRAM();
};

#endif //CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H
