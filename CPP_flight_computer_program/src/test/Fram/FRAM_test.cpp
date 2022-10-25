#include "FRAM_test.h"

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
    uint16_t Counter = 0, Required = 0, Tracker = 0, Addr = 0x60;

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
            // we might need to implement an addres limiter, but 256kb is a lot of address to use
            // will crash the program tho
            const uint8_t Temp_Value = m_FRAM.read(Temp_Addr);

            if(Temp_Value != Zero)
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

    return Addr;
}

std::tuple<std::string, uint16_t, uint8_t> Floating_point::Store(std::string Name)
{
    return std::tuple<std::string, uint16_t, uint8_t>(std::move(Name), m_Addr, m_Size);
}

/* =========================================================================
 *                          f16_FRAM
 * =========================================================================
 */
void f16_FRAM::Write(const float& Value)
{
    if (Value > 127 || Value < -127)
    {
        // return AVR::Result -> AVR_Failed
    }

    std::bitset<8> Whole;
    uint8_t Decimal;
    bool Negative = false;

    // negative values are stored backwards, don't want that
    if (Value < 0)
    {
        Whole = static_cast<uint8_t>(-Value);
        Negative = true;
    }
    else
    {
        Whole = static_cast<uint8_t>(Value);
    }

    // narrowing conversion from int to float -> inaccuracy
    Decimal = static_cast<uint8_t>((Value - Whole.to_ulong() * 1000));

    if (Negative == true)
    {
        // Negative flag
        Whole[0] = 1;
    }

    m_FRAM.write(m_Addr, static_cast<uint8_t>(Whole.to_ulong()));
    m_FRAM.write(m_Addr++, Decimal);

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
    float Val1 = Register1.to_ulong(), Val2 = Register2.to_ulong();

    while(Val2 >= 1)
    {
        Val2 / 10;
    }

    return Negative == true ? -(Val1 + Val2) : (Val1 + Val2);
}

// Operators
f16_FRAM& f16_FRAM::operator = (const f16_FRAM& RHS)
{   if(this != &RHS)
    {
        this->m_Value = RHS.m_Value;
        this->m_Addr = RHS.m_Addr;
    }
    return *this;
}

bool f16_FRAM::operator == (const f16_FRAM& Other) const
{
    return m_Value == Other.m_Value;
}

// Destructor
f16_FRAM::~f16_FRAM()
{
    void Clear();
}

//clear storage
void f16_FRAM::Clear()
{
    const uint8_t Zero = 0x00;

    for(uint8_t i = 0; i < 2; i++)
    {
        m_FRAM.write(m_Addr, Zero);
        m_Addr++;
    }
}

/* =========================================================================
 *                          f32_FRAM
 * =========================================================================
 */

// Operators
f32_FRAM& f32_FRAM::operator = (const float& Value) const
{
    f32_FRAM Temp = Value;
};

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


