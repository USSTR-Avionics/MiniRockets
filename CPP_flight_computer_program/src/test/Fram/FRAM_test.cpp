#include "FRAM_test.h"
/* =========================================================================
 *                          Floating_point
 * =========================================================================
 */
uint16_t Floating_point::Find_Addr(const uint8_t& Size)
{
    uint16_t Counter = 0, Required = 0, Tracker = 0;

    const uint8_t Zero = 0x00;

    bool Flag = false;

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
        for (; Counter < Required; Counter++)
        {
            const uint8_t Temp = FRAM.read(Addr);

            if(Temp != Zero)
            {
                Flag = true;
                Tracker++;
            }
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

bool Floating_point::operator == (const Floating_point& Other) const
{
    return Other.Value == this->Value;
}

Floating_point::Floating_point(const float &Input, const uint8_t Size)
    : Value(Input), Addr(Find_Addr(Size)) {}

/* =========================================================================
 *                          f16_FRAM
 * =========================================================================
 */
f16_FRAM& f16_FRAM::operator = (const f16_FRAM& Other)
{
}

void f16_FRAM::Clear()
{
    const uint8_t Zero = 0x00;

    for(uint8_t i = 0; i < 2; i++)
    {
        FRAM.write(Addr, Zero);
        Addr++;
    }
}

f16_FRAM::~f16_FRAM()
{
    void Clear();
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
        FRAM.write(Addr, Zero);
        Addr++;
    }
}


