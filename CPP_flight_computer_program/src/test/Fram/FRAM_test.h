#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H

#include <tuple>
#include <bitset>
#include <utility>

#include "Adafruit_EEPROM_I2C.h"
#include "Adafruit_FRAM_I2C.h"

// test

class Floating_point
{
public:
    // constructor
    Floating_point(const float &Input, const uint8_t &Bits);

    // IO
    virtual float Read() = 0;

    virtual void Write(const float& Value) = 0;

    // util
    uint8_t Get_Size() const;
    float Get_Value() const;
    // Calling this before writing is undefined behaviour, I'm too lazy to write a check
    // use std::optional for check
    uint16_t Get_Addr() const;

    // Assign m_Addr to empty addr
    void Find_Addr(const uint8_t &Bits, uint16_t Addr);
    // save data address in FRAM to container <"Name", Addr, Size(bits)>
    std::tuple<std::string, uint16_t, uint8_t> Store(std::string Name);

protected:
    // where
    uint16_t m_Addr;
    // size in bits
    uint8_t m_Bits;
    // value stored in decimal
    float m_Value;

    // FRAM API
    Adafruit_FRAM_I2C m_FRAM = Adafruit_FRAM_I2C();
};

// consider removing f16
class f16_FRAM : protected Floating_point
{
    /*
     * [  1       5      10 ]
     * [sign exponents value]
     */
public:
    // Constructors
    f16_FRAM();

    f16_FRAM(const float& Input)
    : Floating_point(Input, 16){};

    f16_FRAM(const f16_FRAM& RHS)
            : Floating_point(RHS.m_Value, 16){};

    // IO
    void Write(const float& Value) override;

    float Read() override;

    // operators

    f16_FRAM operator + (const f16_FRAM &RHS) const;

    f16_FRAM operator - (const f16_FRAM &RHS) const;

    f16_FRAM operator * (const f16_FRAM &RHS) const;

    f16_FRAM operator / (const f16_FRAM &RHS) const;
    // assignment
    f16_FRAM& operator = (const f16_FRAM &RHS);

    // comparison
    bool operator < (const f16_FRAM &RHS) const;
    bool operator > (const f16_FRAM &RHS) const;
    bool operator == (const f16_FRAM &RHS) const;

    // call clear at destruction
    ~f16_FRAM();

private:
    void Clear();

};

class f24_FRAM : protected Floating_point
{
    /*
     * [  1       6      17 ]
     * [sign exponents value]
     */

public:
    // Constructors
    f24_FRAM();

    f24_FRAM(const float& Input)
            : Floating_point(Input, 24){};

    f24_FRAM(const f24_FRAM& RHS)
            : Floating_point(RHS.m_Value, 24){};

    // IO
    void Write(const float& Value) override;

    float Read() override;

    // operators
    // addition
    f24_FRAM& operator + (const float& Value) const;
    f24_FRAM& operator + (const f24_FRAM& RHS) const;
    // subtraction
    f24_FRAM& operator - (const float& Value) const;
    f24_FRAM& operator - (const f24_FRAM& RHS) const;
    // assignment
    f24_FRAM& operator = (const float& Value) const;
    f24_FRAM& operator = (const f24_FRAM& Value) const;
    // comparison
    bool operator == (const f24_FRAM& RHS) const;

    ~f24_FRAM();
private:
    void Clear();
};


class f32_FRAM : protected Floating_point
{
    /*
     * [  1      8      23  ]
     * [sign exponents value]
     */

public:
    // Constructors
    f32_FRAM();

    f32_FRAM(const float& Input)
            : Floating_point(Input, 32){};

    f32_FRAM(const f32_FRAM& RHS)
            : Floating_point(RHS.m_Value, 32){};

    // IO
    void Write(const float& Value) override;

    float Read() override;

    // operators
    // addition
    f32_FRAM& operator + (const float& Value) const;
    f32_FRAM& operator + (const f32_FRAM& RHS) const;
    // subtraction
    f32_FRAM& operator - (const float& Value) const;
    f32_FRAM& operator - (const f32_FRAM& RHS) const;
    // assignment
    f32_FRAM& operator = (const float& Value) const;
    f32_FRAM& operator = (const f32_FRAM& Value) const;
    // comparison
    bool operator == (const f32_FRAM& RHS) const;

    ~f32_FRAM();
private:
    void Clear();
};

#endif //CPP_FLIGHT_COMPUTER_PROGRAM_FRAM_TEST_H
