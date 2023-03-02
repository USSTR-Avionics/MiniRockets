#include "RFM95W.h"

void RFM95W::Init(RH_RF95 &Radio)
{
    // default configuration: 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol(2^7), CRC on
    m_RF95 = &Radio;
    m_Max_message_length = m_RF95->maxMessageLength();
};

bool RFM95W::TCP_Send(const char Data[], const uint16_t &Time_Out_RX, const uint16_t &Time_Out_TX) const
{
    // returns false if message too long
    if (!m_RF95->send(reinterpret_cast<const uint8_t*>(Data), sizeof(*Data))) return false;

    if(m_RF95->waitPacketSent(Time_Out_TX) == true)
    {
        if (m_RF95->waitAvailableTimeout(Time_Out_RX) == true)
        {
            uint8_t Buffer[m_Max_message_length];
            uint8_t Length = sizeof(Buffer);

            // this handshake could happen forever
            if (m_RF95->recv(Buffer, &Length) == true)
            {
                // this is not necessary if RX is only performing handshake
                return std::string{reinterpret_cast<const char *>(Buffer)} == m_Handshake;
            }
            return false;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool RFM95W::UDP_Send(const char Data[]) const
{
    return m_RF95->send(reinterpret_cast<const uint8_t*>(Data), sizeof(*Data));
}

std::string RFM95W::TCP_Receive() const
{
    uint8_t Buffer[m_Max_message_length];
    uint8_t Length = sizeof(Buffer);

    m_RF95->waitAvailable();

    if(m_RF95->recv(Buffer, &Length) == true)
    {
        m_RF95->send(reinterpret_cast<const uint8_t*>(m_Handshake.c_str()), sizeof(m_Handshake.c_str()));

        // this type cast is very funky, functionally the exact same as (const char*) var, but dangerous regardless
        return reinterpret_cast<const char*>(Buffer);
    }
    else
    {
        return "";
    }
}

std::string RFM95W::UDP_Receive() const
{
    uint8_t Buffer[m_Max_message_length];
    uint8_t Length = sizeof(Buffer);

    m_RF95->waitAvailable();

    return m_RF95->recv(Buffer, &Length) ? reinterpret_cast<const char*>(Buffer) : "";

}

std::string RFM95W::TCP_Receive(const uint16_t &Time_Out) const
{
    uint8_t Buffer[m_Max_message_length];
    // isn't this just m_Max_Message_Length + 1
    uint8_t Length = sizeof(Buffer);

    if(m_RF95->waitAvailableTimeout(Time_Out) == true)
    {
        if(m_RF95->recv(Buffer, &Length) == true)
        {
            // TODO perform handshake
            m_RF95->send(reinterpret_cast<const uint8_t*>(m_Handshake.c_str()), sizeof(m_Handshake.c_str()));

            // this type cast is very funky, functionally the exact same as (const char*) var, but dangerous regardless
            return reinterpret_cast<const char*>(Buffer);
        }
        else
        {
            return "";
        }
    }
    else
    {
        // timed out
        return std::string{"Timed out"};
    }

}

std::string RFM95W::UDP_Receive(const uint16_t &Time_Out) const
{
    uint8_t Buffer[m_Max_message_length];
    // isn't this just m_Max_Message_Length + 1
    uint8_t Length = sizeof(Buffer);

    if (m_RF95->waitAvailableTimeout(Time_Out) == true)
    {
        return m_RF95->recv(Buffer, &Length) ? reinterpret_cast<const char*>(Buffer) : "";
    }
    else
    {
        // timed out
        return std::string{"Timed out"};
    }

}

bool RFM95W::Set_Frequency(const float &Frequency) const
{
    return m_RF95->setFrequency(Frequency);
}

void RFM95W::Set_Modem_Config_Choice(const uint8_t &Index) const
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

void RFM95W::Switch_Mode(const RFM95W::Mode &Type) const
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
    return m_Max_message_length;
}

void RFM95W::Set_Preamble_Length(const uint8_t &Length) const
{
    m_RF95->setPreambleLength(Length);
}

bool RFM95W::Mem_check() const
{
    return m_RF95 != nullptr;
}

void RFM95W::Set_TX_Power(const int8_t &Power) const
{
    m_RF95->setTxPower(Power);
}

