#include <RH_RF95.h>

namespace RF95
{
    enum class Mode
    {
        RX,
        TX,
        IDLE
    };

    uint8_t Slave, Interrupt, Reset;

    // default configuration: 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol(2^7), CRC on
    RH_RF95 m_RF95(Slave, Interrupt);

    bool Init = m_RF95.init();

    // why doesn't this work
/*    if (Init == false)
    {
        Serial.println("Failed to initialize radio");
    }*/

    const uint16_t m_Max_message_length = m_RF95.maxMessageLength();
    const std::string m_Handshake{"Received"};

    bool TCP_Send(const char Data[], const uint16_t &Time_Out_RX, const uint16_t &Time_Out_TX)
    {
        // 'atoi()' used to convert a string to an integer value,
        // but function will not report conversion errors; consider using 'strtoul' instead
        // const uint8_t Message = std::atoi(Data);

        // returns false if message too long

        // need to doublecheck to make sure sizeof(*Data) does not return sizeof(Data_ptr)
        m_RF95.send(reinterpret_cast<const uint8_t *>(Data), sizeof(*Data));

        if (m_RF95.waitPacketSent(Time_Out_TX) == true)
        {
            if (m_RF95.waitAvailableTimeout(Time_Out_RX) == true)
            {
                uint8_t Buffer[m_Max_message_length];
                uint8_t Length = sizeof(Buffer);

                // this handshake could happen forever
                if (m_RF95.recv(Buffer, &Length) == true)
                {
                    // this is not necessary if RX is only performing handshake
                    return std::string{reinterpret_cast<const char *>(Buffer)} == m_Handshake;
                }
                return false;
            } else
            {
                return false;
            }
        }
        return false;
    }

    void UDP_Send(const char Data[])
    {
        m_RF95.send(reinterpret_cast<const uint8_t *>(Data), sizeof(*Data));
    }

    std::string Receive()
    {
        uint8_t Buffer[m_Max_message_length];
        uint8_t Length = sizeof(Buffer);

        m_RF95.waitAvailable();

        if (m_RF95.recv(Buffer, &Length) == true)
        {
            m_RF95.send(reinterpret_cast<const uint8_t *>(m_Handshake.c_str()), sizeof(m_Handshake.c_str()));

            // this type cast is very funky, functionally the exact same as (const char*) var, but dangerous regardless
            return reinterpret_cast<const char *>(Buffer);
        } else
        {
            return "";
        }
    }


    std::string Receive(const uint8_t &Time_Out)
    {
        uint8_t Buffer[m_Max_message_length];
        // isn't this just m_Max_Message_Length + 1
        uint8_t Length = sizeof(Buffer);

        if (m_RF95.waitAvailableTimeout(Time_Out) == true)
        {
            if (m_RF95.recv(Buffer, &Length) == true)
            {
                m_RF95.send(reinterpret_cast<const uint8_t *>(m_Handshake.c_str()), sizeof(m_Handshake.c_str()));

                // this type cast is very funky, functionally the exact same as (const char*) var, but dangerous regardless
                return reinterpret_cast<const char *>(Buffer);
            } else
            {
                return "";
            }
        } else
        {
            // timed out
            return "Timed out";
        }

    }

    bool Set_Frequency(const float &Frequency)
    {
        return m_RF95.setFrequency(Frequency);
    }

    void Set_Modem_Config_Choice(const uint8_t &Index)
    {
        switch (Index)
        {
            case 0:
                // this is default already
                m_RF95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
                break;
            case 1:
                m_RF95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
                break;
            case 2:
                m_RF95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
                break;
            case 3:
                m_RF95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
                break;
            default:
                m_RF95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
                break;
        }
    }

    void Switch_Mode(const RFM95W::Mode &Type)
    {
        switch (Type)
        {
            case RFM95W::Mode::RX:
                m_RF95.setModeRx();
                break;

            case RFM95W::Mode::TX:
                m_RF95.setModeTx();
                break;

            case RFM95W::Mode::IDLE:
                m_RF95.setModeIdle();
                break;
        }
    }

    uint16_t Max_Message_Length()
    {
        return m_Max_message_length;
    }

    void Set_Preamble_Length(const uint8_t &Length)
    {
        m_RF95.setPreambleLength(Length);
    }
}