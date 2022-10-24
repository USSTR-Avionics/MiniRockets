#include "FRAM_test.h"

#include <utility>
/* =========================================================================
 *                          Floating_point
 * =========================================================================
 */
// Constructor
Floating_point::Floating_point(const float &Input, const uint8_t& Size)
        : m_Addr(Find_Addr(Size)), m_Size(Size), m_Value(Input) {}

// Member functions
uint16_t Floating_point::Find_Addr(const uint8_t& Size)
{
    uint16_t Counter = 0, Required = 0, Tracker = 0;

    const uint8_t Zero = 0x00;

    // Amount required
    switch(Size)
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
            // throw an error
            break;
    }

    while (true)
    {
        bool Flag = false;

        for (; Counter < Required; Counter++)
        {
            // we might need to implement an addres limiter, but 256kb is a lot of address to use
            // will crash the program tho
            const uint8_t Temp = m_FRAM.read(m_Addr);

            if(Temp != Zero)
            {
                Flag = true;
                Tracker++;
            }
        }

        if(Flag == true)
        {
            m_Addr += Tracker;
            Tracker = 0;
        }
        else
        {
            break;
        }
    }

    return m_Addr;
}

std::tuple<std::string, uint16_t, uint8_t> Floating_point::Store(std::string Name)
{
    return std::tuple<std::string, uint16_t, uint8_t>(std::move(Name), m_Addr, m_Size);
}

// Operators
bool Floating_point::operator == (const Floating_point& Other) const
{
    return m_Value == Other.m_Value;
}


/* =========================================================================
 *                          f16_FRAM
 * =========================================================================
 */
void Write(const float& Value)
{

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
        // shift left to remove flag
        Register1 << 1;
    }
    else
    {
        // shift left to remove flag
        Register1 << 1;
    }

}

// Destructor
f16_FRAM::~f16_FRAM()
{
    void Clear();
}

void f16_FRAM::Clear()
{
    const uint8_t Zero = 0x00;

    for(uint8_t i = 0; i < 2; i++)
    {
        m_FRAM.write(m_Addr, Zero);
        m_Addr++;
    }
}

// Operator
f16_FRAM& f16_FRAM::operator = (const f16_FRAM& Other)
{
}


/* =========================================================================
 *                          f32_FRAM
 * =========================================================================
 */

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


