#ifndef AVIONICS_RX_RFM95W_H
#define AVIONICS_RX_RFM95W_H

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


     Settings:
        It is very important therefore, that if you are using the RH_RF95 driver with another SPI based device,
        that you disable interrupts while you transfer data o and from that other device.
        Use cli() to disable interrupts and sei() to readable them.
*/
#include <RH_RF95.h>
#include <string>

// Must call Init() before doing anything else
class RFM95W
{
public:
    enum class Mode
    {
        RX,
        TX,
        IDLE
    };

    // core
    void Init(RH_RF95 &Radio);

    // send
    bool TCP_Send(const char Data[], const uint16_t &Time_Out_RX, const uint16_t &Time_Out_TX = 500) const;
    bool UDP_Send(const char Data[]) const;

    // Receive
    std::string TCP_Receive() const;
    std::string UDP_Receive() const;

    // Time_Out in milliseconds
    std::string TCP_Receive(const uint16_t &Time_Out) const;
    std::string UDP_Receive(const uint16_t &Time_Out) const;

    // util
    bool Mem_check() const;
    uint16_t Max_Message_Length() const;
    bool Set_Frequency(const float &Frequency) const;
    void Set_TX_Power(const int8_t &Power) const;
    void Switch_Mode(const Mode &Type) const;
    // default RH_RF95::Bw125Cr45Sf128
    void Set_Modem_Config_Choice(const uint8_t &Index) const;
    // in bytes
    void Set_Preamble_Length(const uint8_t &Length) const;


private:
    // obj is passed in, no need to call delete
    RH_RF95 *m_RF95{nullptr};
    uint16_t m_Max_message_length{};
    const std::string m_Handshake{"Received"};
};

#endif //AVIONICS_RX_RFM95W_H
