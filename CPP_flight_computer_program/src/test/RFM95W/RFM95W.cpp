#include "RFM95W.h"

RFM95W::RFM95W(const uint8_t &Slave, const uint8_t &Interrupt)
{
    RF95 = std::make_unique<RH_RF95>(Slave, Interrupt);

    // default configuration: 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol(2^7), CRC on
    RF95->init();
}

bool RFM95W::Send(const std::string &Data)
{
    // 'atoi' used to convert a string to an integer value,
    // but function will not report conversion errors; consider using 'strtol' instead
    const uint8_t Message = std::atoi(Data.c_str());

    // returns false if message too long
    return RF95->send(&Message, sizeof(Message));
}

bool RFM95W::Set_Frequency(const float &Frequency)
{
    return RF95->setFrequency(Frequency);
}

void RFM95W::Set_Modem_Config_Choice(const uint8_t &Index)
{
    switch(Index)
    {
        case 0:
            // this is default already
            RF95->setModemConfig(RH_RF95::Bw125Cr45Sf128);
            break;
        case 1:
            RF95->setModemConfig(RH_RF95::Bw500Cr45Sf128);
            break;
        case 2:
            RF95->setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
            break;
        case 3:
            RF95->setModemConfig(RH_RF95::Bw125Cr48Sf4096);
            break;
        default:
            RF95->setModemConfig(RH_RF95::Bw125Cr45Sf128);
            break;
    }
}

bool RFM95W::Recieve()
{
    return false;
}




