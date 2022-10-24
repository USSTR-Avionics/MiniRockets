#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H

#include <tuple>
#include <string>
#include <bitset>

#include "Adafruit_EEPROM_I2C.h"
#include "Adafruit_FRAM_I2C.h"

// test

class Floating_point
{
public:
    // constructor
    Floating_point(const float& Input, const uint8_t& Size);

    // IO
    virtual float Read() = 0;

    virtual void Write(const float& Value) = 0;

    // util
    uint16_t Find_Addr(const uint8_t& Size);
    // save data address in FRAM to container <"Name", Addr, Size(bits)>
    std::tuple<std::string, uint16_t, uint8_t> Store(std::string Name);

    // operators
    // equal
    bool operator == (const Floating_point& Other) const;

protected:
    // where
    uint16_t m_Addr = 0x60;
    // how big
    uint8_t m_Size = 8;
    // value stored in decimal
    float m_Value = 0;

    // FRAM API
    Adafruit_FRAM_I2C m_FRAM = Adafruit_FRAM_I2C();
};
class f16_FRAM : protected Floating_point
{
public:
    // Constructors
    f16_FRAM();

    f16_FRAM(const float& Input)
    : Floating_point(Input, 16){};

    // IO
    void Write(const float& Value) override;

    float Read() override;

    // operators
    // addition
    f16_FRAM& operator + (const float& Value) const;
    f16_FRAM& operator + (const f16_FRAM& Other) const;
    // subtraction
    f16_FRAM& operator - (const float& Value) const;
    f16_FRAM& operator - (const f16_FRAM& Other) const;
    // assignment
    f16_FRAM& operator = (const f16_FRAM& Value) const;

    // comparison
    bool operator == (const f16_FRAM& Other) const;

    // call clear at destruction
    ~f16_FRAM();

private:
    void Clear();

};

class f32_FRAM : protected Floating_point
{
public:
    // Constructors
    f32_FRAM();

    f32_FRAM(const float& Input)
            : Floating_point(Input, 32){};

    // IO
    void Write(const float& Value) override;

    float Read() override;

    // operators
    // addition
    f32_FRAM& operator + (const float& Value) const;
    f32_FRAM& operator + (const f32_FRAM& Other) const;
    // subtraction
    f32_FRAM& operator - (const float& Value) const;
    f32_FRAM& operator - (const f32_FRAM& Other) const;
    // assignment
    f32_FRAM& operator = (const float& Value) const;
    f32_FRAM& operator = (const f32_FRAM& Value) const;
    // comparison
    bool operator == (const f32_FRAM& Other) const;

    ~f32_FRAM();
private:
    void Clear();
};

#endif //CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H
