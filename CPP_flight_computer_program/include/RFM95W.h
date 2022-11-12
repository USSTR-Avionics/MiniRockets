#ifndef CPP_FLIGHT_COMPUTER_PROGRAM_RFM95W_H
#define CPP_FLIGHT_COMPUTER_PROGRAM_RFM95W_H

// YOU CAN ONLY HAVE 2 INSTANCES OF THIS OBJ AT 1 TIME (3 IF MEGA)
/*
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
#include <RH_RF95.h>
#include <memory>
#include <cstring>
#include <tuple>

class RFM95W
{
public:
    enum Mode
    {
        RX,
        TX,
        IDLE
    };

    RFM95W(const uint8_t &Slave, const uint8_t &Interrupt, const Mode &Type = Mode::IDLE);


    // core
    bool Send(const char *Data[], const uint16_t &Time_Out_TX, const uint16_t &Time_Out_RX) const;

    std::tuple<bool, const char*> Receive();
    // Time_Out in milliseconds
    std::tuple<bool, const char*> Receive(const uint8_t &Time_Out);

    // util
    uint16_t Max_Message_Length() const;


    // Settings
    // It is very important therefore, that if you are using the RH_RF95 driver with another SPI based deviced,
    // that you disable interrupts while you transfer data to and from that other device.
    // Use cli() to disable interrupts and sei() to reenable them.
    bool Set_Frequency(const float &Frequency);
    // default RH_RF95::Bw125Cr45Sf128
    void Set_Modem_Config_Choice(const uint8_t &Index);
    // in bytes
    void Set_Preamble_Length(const uint8_t &Length);
    void Switch_Mode(const Mode &Type);


    RFM95W() = delete;
    RFM95W(const RFM95W &RHS) = delete;

private:
    std::unique_ptr<RH_RF95> m_RF95;
    uint16_t m_Max_Message_Length{};
    constexpr static char m_HandShake[]{"Received"};
};

#endif //CPP_FLIGHT_COMPUTER_PROGRAM_RFM95W_H