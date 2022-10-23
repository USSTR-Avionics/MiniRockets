#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H

#include "Adafruit_EEPROM_I2C.h"
#include "Adafruit_FRAM_I2C.h"

// test

class Floating_point
{
public:
    Floating_point(const float& Input, const uint8_t Size);

    bool operator == (const Floating_point& Other) const;

    virtual Floating_point& operator + (const Floating_point& Other) = 0;

    uint16_t Find_Addr(const uint8_t& Size);

    virtual void Write(const float& Value) = 0;

    virtual float Read() = 0;

    // where
    uint16_t Addr = 0x60;
    // how big
    float Value;

    Adafruit_FRAM_I2C FRAM = Adafruit_FRAM_I2C();

private:
    virtual void Clear() = 0;

};
class f16_FRAM : protected Floating_point
{
    f16_FRAM(const float& Input, const uint8_t& Size)
    : Floating_point(Input, Size){};

    void operator = (const float& Value);

    f16_FRAM& operator = (const f16_FRAM& Other);

    f16_FRAM& operator + (const f16_FRAM& Other);

    void Write(const float& Value) override;

    float Read();

    // call write at destruction
    ~f16_FRAM();
private:
    void Clear() override;

};

class f32_FRAM : protected Floating_point
{

    ~f32_FRAM();
private:
    void Clear() override;
};

#endif //CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H
