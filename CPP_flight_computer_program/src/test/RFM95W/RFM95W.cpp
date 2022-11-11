#include "RFM95W.h"

RFM95W::RFM95W(const uint8_t &Slave, const uint8_t &Interrupt, const Mode &Type)
{
    m_RF95 = std::make_unique<RH_RF95>(Slave, Interrupt);
    // default configuration: 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol(2^7), CRC on
    m_RF95->init();

    Switch_Mode(Type);

    m_Max_Message_Length = m_RF95->maxMessageLength();
}

bool RFM95W::Send(const char *Data[], const uint16_t &Time_Out_TX, const uint16_t &Time_Out_RX) const
{
    // 'atoi()' used to convert a string to an integer value,
    // but function will not report conversion errors; consider using 'strtoul' instead
    // const uint8_t Message = std::atoi(Data);

    // returns false if message too long

    // need to double check to make sure sizeof(*Data) does not return sizeof(Data_ptr)
    m_RF95->send(reinterpret_cast<const uint8_t*>(Data), sizeof(*Data));

    if(m_RF95->waitPacketSent(Time_Out_TX) == true)
    {
        // TODO hand shake
        if (m_RF95->waitAvailableTimeout(Time_Out_RX) == true)
        {
            uint8_t Buffer[m_Max_Message_Length];
            uint8_t Length = sizeof(Buffer);

            // this hand shake could happen forever
            if (m_RF95->recv(Buffer, &Length) == true)
            {
                // this is not necessary if RX is only performing handshake
                if (strcmp(reinterpret_cast<const char *>(Buffer), m_HandShake) == 0)
                {
                    return true;
                }
                else
                {
                    return Send(Data, Time_Out_TX, Time_Out_RX);
                }
            }
            return false;
        }
        else
        {
            return Send(Data, Time_Out_TX, Time_Out_RX);
        }
    }
    return false;
}

std::tuple<bool,  const char*> RFM95W::Received()
{
    uint8_t Buffer[m_Max_Message_Length];
    uint8_t Length = sizeof(Buffer);

    m_RF95->waitAvailable();

    if(m_RF95->recv(Buffer, &Length) == true)
    {
        // TODO perform handshake
        m_RF95->send(reinterpret_cast<const uint8_t*>(m_HandShake), sizeof(m_HandShake));

        // this type cast is very funky, functionally the exact same as (const char*) var, but dangerous regardless
        return std::make_tuple(true, reinterpret_cast<const char*>(Buffer));
    }
    else
    {
        return std::make_tuple(false, "Failed to receive message");
    }
}


std::tuple<bool,  const char*> RFM95W::Received(const uint8_t &Time_Out)
{
    uint8_t Buffer[m_Max_Message_Length];
    // isn't this just m_Max_Message_Length + 1
    uint8_t Length = sizeof(Buffer);

    if(m_RF95->waitAvailableTimeout(Time_Out) == true)
    {
        if(m_RF95->recv(Buffer, &Length) == true)
        {
            // TODO perform handshake
            m_RF95->send(reinterpret_cast<const uint8_t*>(m_HandShake), sizeof(m_HandShake));

            // this type cast is very funky, functionally the exact same as (const char*) var, but dangerous regardless
            return std::make_tuple(true, reinterpret_cast<const char*>(Buffer));
        }
        else
        {
            return std::make_tuple(false, "Failed to receive message");
        }
    }
    else
    {
        // timed out
        return std::make_tuple(false, "Timed out");
    }

}

bool RFM95W::Set_Frequency(const float &Frequency)
{
    return m_RF95->setFrequency(Frequency);
}

void RFM95W::Set_Modem_Config_Choice(const uint8_t &Index)
{
    switch(Index)
    {
        case 0:
            // this is default already
            m_RF95->setModemConfig(RH_RF95::Bw125Cr45Sf128);
            break;
        case 1:
            m_RF95->setModemConfig(RH_RF95::Bw500Cr45Sf128);
            break;
        case 2:
            m_RF95->setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
            break;
        case 3:
            m_RF95->setModemConfig(RH_RF95::Bw125Cr48Sf4096);
            break;
        default:
            m_RF95->setModemConfig(RH_RF95::Bw125Cr45Sf128);
            break;
    }
}

void RFM95W::Switch_Mode(const RFM95W::Mode &Type)
{
    switch(Type)
    {
        case RFM95W::Mode::RX:
            m_RF95->setModeRx();
            break;

        case RFM95W::Mode::TX:
            m_RF95->setModeTx();
            break;

        case RFM95W::Mode::IDLE:
            m_RF95->setModeIdle();
            break;
    }
}

uint16_t RFM95W::Max_Message_Length() const
{
    return m_Max_Message_Length;
}

void RFM95W::Set_Preamble_Length(const uint8_t &Length)
{
    m_RF95->setPreambleLength(Length);
}




