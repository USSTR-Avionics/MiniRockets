#include "FRAM_test.h"

/* =========================================================================
 *                          Floating_point
 * =========================================================================
 */
// Constructor
Floating_point::Floating_point(const float &Input, const uint8_t &Bits)
        : m_Bits(Bits), m_Value(Input) {}

// Member functions
AVR::Result Floating_point::Find_Addr(const uint8_t &Bits, uint16_t Begin_Addr)
{
    uint16_t Counter = 0, Required, Tracker = 0;

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

    uint16_t Temp_Addr = Begin_Addr;
    while (true)
    {
        bool Has_value = false;

        for (; Counter < Required; Counter++)
        {
            /*
             * will crash the program if it goes out of memory, unless adafruit made it wrap around
             * in which case, Find_Addr() will search forever which is also bad
            */

            const uint8_t Temp_Value = m_FRAM.read(Temp_Addr);

            if(Temp_Value != 0)
            {
                Has_value = true;
                Tracker += ++Counter;
            }

            // could limit it to 32k, and use the 768 address to do something else like storing a lookup table
            if(++Temp_Addr > 32768)
            {
                // exceeding all available memory addresses
                return AVR::Result::AVR_FAILED;
            }
        }

        if(Has_value == true)
        {
            Begin_Addr += Tracker;
            Tracker = 0;
        }
        else
        {
            break;
        }
    }

    m_Addr = Begin_Addr;

    return AVR::Result::AVR_SUCCESS;
}

std::tuple<std::string, uint16_t, uint8_t> Floating_point::Store(std::string Name)
{
    return std::tuple<std::string, uint16_t, uint8_t>(std::move(Name), m_Addr, m_Bits);
}

float Floating_point::Get_value() const
{
    return m_Value;
}

uint16_t Floating_point::Get_addr() const
{
    return m_Addr;
}

uint8_t Floating_point::Get_size() const
{
    return m_Bits;
}

/* =========================================================================
 *                          f16_FRAM
 * =========================================================================
 */
AVR::Result f16_FRAM::Write(float& Value)
{
    // Find empty addr when start writing
    if(Find_Addr(m_Bits, 0x60) != AVR::Result::AVR_SUCCESS)
    {
        return AVR::Result::AVR_FAILED;
    }

    // bias
    int8_t Exponent_value = 15;

    std::bitset<16> bWhole, bDecimal;
    std::bitset<32> Mantissa = 0;
    std::bitset<5> Exponent;

    // where everything comes together
    std::bitset<8> Register1, Register2;

    // Don't want to deal with 2's complement, so it's going to be all positive
    // any stored value that exceeds 16 bits will be cut off, could round

    if (Value < 0)
    {
        Value = -Value;

        // sign bit
        Register1[7] = 1;
    }

    while (Value >= 2)
    {
        Value /= 2;
        Exponent_value++;
    }
    while (Value < 1)
    {
        Value *= 2;
        Exponent_value--;
    }
    bWhole = static_cast<uint16_t>(Value);

    // decimal conversion
    float Decimal = Value - static_cast<float>(bWhole.to_ulong());

    int8_t Decimal_bits = 16;
    if (Decimal != 0)
    {
        // compute decimal to 16 bit digits
        for (int16_t i = 15; i >= 0; i--)
        {
            // Find next bit in fraction
            Decimal *= 2;

            // if it is over 1
            if (Decimal > 1.f)
            {
                Decimal -= 1;
                bDecimal[i] = 1;
            } else
            {
                bDecimal[i] = 0;
            }
        }
    } else
    {
        Decimal_bits = 0;
    }

    // Find exponent
    Exponent_value -= Decimal_bits;


    // Integer trim
    uint8_t Integer_bits = 0;
    if (bWhole.to_ulong() > 0)
    {
        // how many bits are integers
        for (int16_t i = 16; i > 0; i--)
        {
            if (bWhole[i] == 1)
            {
                break;
            } else
            {
                Integer_bits = i;
            }
        }
    }

    // 11 1111 1111 = 1023, max mantissa value. (Mantissa > Max) = Max
    // put the two together
    uint8_t Temp_i = 0, Temp_j;
    for (; Temp_i < Decimal_bits; Temp_i++)
    {
        Mantissa[Temp_i] = bDecimal[Temp_i];
    }
    for (Temp_j = Temp_i; Temp_j < (Decimal_bits + Integer_bits); Temp_j++)
    {
        Mantissa[Temp_j] = bWhole[Temp_j - Temp_i];
    }

    // Mantissa cannot be bigger than 10
    int8_t Mantissa_bits = Integer_bits + Decimal_bits;
    if (Mantissa_bits > 10)
    {
        uint8_t Shifts = Mantissa_bits - 10;

        // rounding, optional
        if (Mantissa[Shifts - 1] == 1)
        {
            // rounding
            Mantissa = Mantissa.to_ulong() + 1;

            int8_t Shifts2 = Mantissa_bits - 10;
            Mantissa = Mantissa >> Shifts2;
            Exponent_value += Shifts2;
        } else
        {
            Mantissa = Mantissa >> Shifts;
            Exponent_value += Shifts;
        }
    }

    // same as std::clamp
    if (Exponent_value > 31)
    {
        Exponent_value = 31;
    }
    if (Exponent_value < 0)
    {
        Exponent_value = 0;
    }

    // turn exponent value into bitset
    Exponent = Exponent_value;

    // load Register1, Register2
    for (int16_t i = 0; i < 8; i++)
    {
        Register2[i] = Mantissa[i];

        if (i < 2)
        {
            Register1[i] = Mantissa[8 + i];
        } else if (i < 7)
        {
            Register1[i] = Exponent[i - 2];
        }

        // the negative bit is already set
    }

    // write to memory
    uint8_t Temp_addr = m_Addr;

    if(!m_FRAM.write(Temp_addr, static_cast<uint8_t>(Register1.to_ulong()))
        || !m_FRAM.write(++Temp_addr, static_cast<uint8_t>(Register2.to_ulong())))
    {
        return AVR::Result::AVR_FAILED;
    }

    return AVR::Result::AVR_SUCCESS;
}


float f16_FRAM::Read()
{
    std::bitset<8> Register1, Register2;
    std::bitset<5> Exponents;
    std::bitset<10> Mantissa;
    uint16_t Temp_addr = m_Addr;

    // Assuming this works, pretty sure doesn't
    Register1 = m_FRAM.read(Temp_addr);
    Register2 = m_FRAM.read(++Temp_addr);

    // extract exponents
    for(int16_t i = 2; i < 7; i++)
    {
        Exponents[i - 2] = Register1[i];
    }

    for(int16_t i = 0; i < 8; i++)
    {
        if(i < 2)
        {
            Mantissa[8 + i] = Register1[i];
        }

        Mantissa[i] = Register2[i];
    }

    const auto Value = static_cast<float>(Mantissa.to_ulong() * std::pow(2, static_cast<int>(Exponents.to_ulong() - 15)));

    return Register1[7] == 1 ? -Value : Value;
}

// Operators
f16_FRAM f16_FRAM::operator + (const f16_FRAM &RHS)
{
    return f16_FRAM {m_Value + RHS.m_Value};
}

f16_FRAM f16_FRAM::operator - (const f16_FRAM &RHS)
{
    return f16_FRAM {m_Value - RHS.m_Value};
}

f16_FRAM f16_FRAM::operator * (const f16_FRAM &RHS)
{
    return f16_FRAM {m_Value * RHS.m_Value};
}

f16_FRAM f16_FRAM::operator / (const f16_FRAM &RHS)
{
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
    m_FRAM.write(m_Addr, 0);
    m_FRAM.write(++m_Addr, 0);
}


/* =========================================================================
 *                          f32_FRAM
 * =========================================================================
 */

// I/O
AVR::Result f32_FRAM::Write(float &Value)
{
    if(Find_Addr(m_Bits, 0x60) != AVR::Result::AVR_SUCCESS)
    {
        return AVR::Result::AVR_FAILED;
    }

    union Temp
    {
        float Value;
        char Bytes[4];
    } Temp{};

    Temp.Value = Value;

    for(auto &iter : Temp.Bytes)
    {
        m_FRAM.write(m_Addr, iter);
        m_Addr++;
    }

    return AVR::Result::AVR_SUCCESS;
}

// read
float f32_FRAM::Read()
{
    uint8_t Temp_addr = m_Addr;
    union Temp
    {
        float Value;
        char Bytes[4];
    } Temp{};

    for(auto &iter : Temp.Bytes)
    {
        iter = m_FRAM.read(Temp_addr);
        Temp_addr++;
    }
    return Temp.Value;
}

// Operators
f32_FRAM f32_FRAM::operator + (const f32_FRAM &RHS)
{
    return f32_FRAM {m_Value + RHS.m_Value};
}

f32_FRAM f32_FRAM::operator - (const f32_FRAM &RHS)
{
    return f32_FRAM {m_Value - RHS.m_Value};
}

f32_FRAM f32_FRAM::operator * (const f32_FRAM &RHS)
{
    return f32_FRAM {m_Value * RHS.m_Value};
}

f32_FRAM f32_FRAM::operator / (const f32_FRAM &RHS)
{
    return f32_FRAM {m_Value / RHS.m_Value};
}


f32_FRAM& f32_FRAM::operator = (const f32_FRAM &RHS)
{   if(this != &RHS)
    {
        this->m_Value = RHS.m_Value;
        this->m_Addr = RHS.m_Addr;
    }
    return *this;
}

bool f32_FRAM::operator < (const f32_FRAM &RHS) const
{
    return m_Value < RHS.m_Value;
}

bool f32_FRAM::operator > (const f32_FRAM &RHS) const
{
    return m_Value > RHS.m_Value;
}

bool f32_FRAM::operator == (const f32_FRAM& RHS) const
{
    return m_Value == RHS.m_Value;
}

f32_FRAM::~f32_FRAM()
{
    void Clear();
}
void f32_FRAM::Clear()
{
    for(uint8_t i = 0; i < 4; i++)
    {
        m_FRAM.write(m_Addr, 0);
        m_Addr++;
    }
}


