#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_RFM95W_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_RFM95W_H

/*
YOU CAN ONLY HAVE 2 INSTANCES OF THIS OBJ AT 1 TIME (3 IF MEGA)

    LoRa packet format:
        8 symbol PREAMBLE
        Explicit header with header CRC (default CCITT, handled internally by the radio)
        4 octets HEADER: (TO, FROM, ID, FLAGS)
        0 to 251 octets DATA
        CRC (default CCITT, handled internally by the radio)

    LoRa Chirp Options:
        typedef enum
        {
            Bw125Cr45Sf128 = 0,	   //< Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
            Bw500Cr45Sf128,	           //< Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
            Bw31_25Cr48Sf512,	   //< Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
            Bw125Cr48Sf4096,           //< Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range
        } ModemConfigChoice;
*/
#include <string>

#include <RH_RF95.h>

class RFM95W_v1
{
public:
    enum class Mode
    {
        RX,
        TX,
        IDLE
    };

    RFM95W(const uint8_t &Slave, const uint8_t &Interrupt, const uint8_t &Reset, const Mode &Type = Mode::IDLE);

    // core
    bool TCP_Send(const std::string &Data, const uint16_t &Time_Out_RX, const uint16_t &Time_Out_TX = 500) const;
    void UDP_Send(const std::string &Data) const;

    std::string Receive();

    // Time_Out in milliseconds
    std::string Receive(const uint8_t &Time_Out);

    // util
    uint16_t Max_Message_Length() const;

    /*
     * Settings
        It is very important therefore, that if you are using the RH_RF95 driver with another SPI based device,
        that you disable interrupts while you transfer data o and from that other device.
        Use cli() to disable interrupts and sei() to readable them.
     */
    bool Set_Frequency(const float &Frequency);
    // default RH_RF95::Bw125Cr45Sf128
    void Set_Modem_Config_Choice(const uint8_t &Index);
    // in bytes
    void Set_Preamble_Length(const uint8_t &Length);
    void Switch_Mode(const Mode &Type);

    ~RFM95W();
    RFM95W() = delete;
    RFM95W(const RFM95W &RHS) = delete;

private:
    RH_RF95 *m_RF95;
    uint8_t m_RST;
    uint16_t m_Max_message_length;
    const std::string m_Handshake{"Received"};
};
/*
*//*
 * ========================================================================
 *                                 UDP
 * ========================================================================
 *//*

// I could also just inherit from RFM95W
class RFM95W_TX
{
public:
    enum class Mode
    {
        RX,
        TX,
        IDLE
    };

    RFM95W_TX(const uint8_t &Slave, const uint8_t &Interrupt, const uint8_t &Reset, const Mode &Type = Mode::IDLE);

    // core
    bool Send(const std::string &Data, const uint16_t &Time_Out_TX) const;

    // util
    uint16_t Max_Message_Length() const;


    *//*
     * Settings
        It is very important therefore, that if you are using the RH_RF95 driver with another SPI based device,
        that you disable interrupts while you transfer data o and from that other device.
        Use cli() to disable interrupts and sei() to readable them.
     *//*
    bool Set_Frequency(const float &Frequency);
    // default RH_RF95::Bw125Cr45Sf128
    void Set_Modem_Config_Choice(const uint8_t &Index);
    // in bytes
    void Set_Preamble_Length(const uint8_t &Length);
    void Switch_Mode(const Mode &Type);


    RFM95W_TX() = delete;
    RFM95W_TX(const RFM95W_TX &RHS) = delete;

private:
    std::unique_ptr<RH_RF95> m_RF95;
    uint8_t m_RST;
    uint16_t m_Max_message_length;
};

class RFM95W_RX
{
public:
    enum class Mode
    {
        RX,
        TX,
        IDLE
    };

    RFM95W_RX(const uint8_t &Slave, const uint8_t &Interrupt, const uint8_t &Reset, const Mode &Type = Mode::IDLE);

    // core
    std::tuple<bool, const std::string> Receive();

    // Time_Out in milliseconds
    std::tuple<bool, const std::string> Receive(const uint8_t &Time_Out);

    // util
    uint16_t Max_Message_Length() const;


    *//*
     * Settings
        It is very important therefore, that if you are using the RH_RF95 driver with another SPI based device,
        that you disable interrupts while you transfer data o and from that other device.
        Use cli() to disable interrupts and sei() to readable them.
     *//*
    bool Set_Frequency(const float &Frequency);
    // default RH_RF95::Bw125Cr45Sf128
    void Set_Modem_Config_Choice(const uint8_t &Index);
    // in bytes
    void Set_Preamble_Length(const uint8_t &Length);
    void Switch_Mode(const Mode &Type);


    RFM95W_RX() = delete;
    RFM95W_RX(const RFM95W_RX &RHS) = delete;

private:
    std::unique_ptr<RH_RF95> m_RF95;
    uint8_t m_RST;
    uint16_t m_Max_message_length;
};*/

#endif //CPP_FLIGHT_COMPUTER_PROGRAM_RFM95W_H