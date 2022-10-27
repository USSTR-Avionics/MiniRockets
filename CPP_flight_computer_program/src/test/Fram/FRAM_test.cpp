#include "FRAM_test.h"

/* =========================================================================
 *                          Floating_point
 * =========================================================================
 */
// Constructor
Floating_point::Floating_point(const float &Input, const uint8_t &Bits)
        : m_Bits(Bits), m_Value(Input) {}

// Member functions
void Floating_point::Find_Addr(const uint8_t &Bits, uint16_t Addr)
{
    uint16_t Counter = 0, Required = 0, Tracker = 0;

    // Amount required
    switch(Bits)
    {
        case 8:
            Required = 1;
            break;

        case 16:
            Required = 2;
            break;

        case 24:
            Required = 3;
            break;

        case 32:
            Required = 4;
            break;

        default:
            // this should be an error, but exceptions are disabled cannot throw
            Required = 2;
            break;
    }

    while (true)
    {
        uint16_t Temp_Addr = Addr;

        bool Flag = false;

        for (; Counter < Required; Counter++)
        {
            /* we might need to implement an addres limiter, but 256kb is a lot of address to use
             * will crash the program if goes out of memory, unless adafruit made it wrap around
             * in which case, Find_Addr() will search forever which is also bad
            */

            const uint8_t Temp_Value = m_FRAM.read(Temp_Addr);

            if(Temp_Value != 0)
            {
                Flag = true;
                Tracker += ++Counter;
            }
            Temp_Addr++;
        }

        if(Flag == true)
        {
            Addr += Tracker;
            Tracker = 0;
        }
        else
        {
            break;
        }
    }

    m_Addr = Addr;
}

std::tuple<std::string, uint16_t, uint8_t> Floating_point::Store(std::string Name)
{
    return std::tuple<std::string, uint16_t, uint8_t>(std::move(Name), m_Addr, m_Bits);
}

float Floating_point::Get_Value() const
{
    return m_Value;
}

uint16_t Floating_point::Get_Addr() const
{
    return m_Addr;
}

uint8_t Floating_point::Get_Size() const
{
    return m_Bits;
}

/* =========================================================================
 *                          f16_FRAM
 * =========================================================================
 */
void f16_FRAM::Write(const float& Value)
{
    // Assign m_Addr to empty addr
    Find_Addr(m_Bits, 0x60);

    // rewrite this whole thing according to IEEE 754 floating point
    if (Value > 127.127 || Value < -127.127)
    {
        // return AVR::Result -> AVR_Failed
    }

    std::bitset<8> Register1, Register2;
    uint16_t Whole, Decimal, Exponents;
    bool Negative;

    // negative values are stored backwards, don't want that
    if (Value < 0)
    {
        Whole = static_cast<uint16_t>(-Value);
        Negative = true;
    }
    else
    {
        Whole = static_cast<uint16_t>(Value);
    }

    // narrowing conversion from int to float
    Decimal = static_cast<uint16_t>((Value - Whole) * 1000);

    // Find exponent
    while (Whole > 2)
    {
        Whole /= 2;
        Exponents++;
    }

    if (Negative == true)
    {
        // Set negative flag

    }

    // m_FRAM.write(m_Addr, static_cast<uint8_t>(Whole.to_ulong()));
    // m_FRAM.write(m_Addr++, Decimal);

    // return AVR::Result -> AVR_SUCCESS

}

float f16_FRAM::Read()
{
    std::bitset<8> Register1, Register2;
    bool Negative = false;

    // Assuming this works, pretty sure doesn't
    Register1 = m_FRAM.read(m_Addr);
    Register2 = m_FRAM.read(m_Addr++);

    if(Register1[0] == 1)
    {
        Negative = true;
        Register1[0] = 0;
    }

    // chain
    // these casts will result in inaccuracy 99%
    float Whole = Register1.to_ulong(), Decimal = Register2.to_ulong();

    while(Decimal >= 1)
    {
        Decimal /= 10;
    }

    return Negative == true ? -(Whole + Decimal) : (Whole + Decimal);
}

// Operators
f16_FRAM f16_FRAM::operator + (const f16_FRAM &RHS) const
{
    return f16_FRAM {m_Value + RHS.m_Value};
}

f16_FRAM f16_FRAM::operator - (const f16_FRAM &RHS) const
{
    return f16_FRAM {m_Value - RHS.m_Value};
}

f16_FRAM f16_FRAM::operator * (const f16_FRAM &RHS) const {
    return f16_FRAM {m_Value * RHS.m_Value};
}

f16_FRAM f16_FRAM::operator / (const f16_FRAM &RHS) const {
    return f16_FRAM {m_Value / RHS.m_Value};
}


f16_FRAM& f16_FRAM::operator = (const f16_FRAM &RHS)
{   if(this != &RHS)
    {
        this->m_Value = RHS.m_Value;
        this->m_Addr = RHS.m_Addr;
    }
    return *this;
}

bool f16_FRAM::operator < (const f16_FRAM &RHS) const
{
    return m_Value < RHS.m_Value;
}

bool f16_FRAM::operator > (const f16_FRAM &RHS) const
{
    return m_Value > RHS.m_Value;
}

bool f16_FRAM::operator == (const f16_FRAM& RHS) const
{
    return m_Value == RHS.m_Value;
}

// Destructor
f16_FRAM::~f16_FRAM()
{
    void Clear();
}

//clear storage
void f16_FRAM::Clear()
{
    // overwrite the values with 0
    for(uint8_t i = 0; i < 2; i++)
    {
        m_FRAM.write(m_Addr, 0);
        m_Addr++;
    }
}


/* =========================================================================
 *                          f32_FRAM
 * =========================================================================
 */

// Operators
/*
f32_FRAM& f32_FRAM::operator = (const float& Value) const
{
    f32_FRAM Temp = Value;
};
*/

bool f32_FRAM::operator == (const f32_FRAM& Other) const
{
    return m_Value == Other.m_Value;
}

f32_FRAM::~f32_FRAM()
{
    void Clear();
}
void f32_FRAM::Clear()
{
    const uint8_t Zero = 0x00;

    for(uint8_t i = 0; i < 4; i++)
    {
        m_FRAM.write(m_Addr, Zero);
        m_Addr++;
    }
}


