/*
 * RFM95W Module
 * 
 * Product Link: https://www.smart-prototyping.com/image/cache/data/2020/10/102074%20MS5611/3-750x750.JPG
 * 
 * Inputs: VIN, GND, EN, G0, SCK, MISO, MOSI, CS, RST, G1, G2, G3, G4, G5
 * 
 * Outputs:
 * - Packet sent to receiver
 * 
 * Set Up Instructions: 
 * 1. Connect VIN to 5V and GND to GND
 * 2. Connect G0INT to pin 25 of TEENSY
 * 3. Connect SCK to pin 13, MISO to pin 12, MOSI to pin 11 (TEENSY)
 * 4. Connect CS to pin 10, RST to pin 24 (TEENSY)
 *
 * Functions:
 * bool iinit_RFM95_TX(): initializes the sensor (0=error, 1=success)
 * sendPacket(): sends a packet to the receiver
 */

// LoRa 9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX



#define RFM95_CS 10
#define RFM95_RST 24
#define RFM95_INT 25

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

bool init_RFM95_TX()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

//void setup() 
//{
//  init_RFM95_TX();
//}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void sendPacket()
{  
  String myPacket = "";
  myPacket.concat(x);
  myPacket.concat(",");
  myPacket.concat(kx134_accel_x);
  
  //char radiopacket[20] = "Hello World #      ";
  char radiopacket[20];
  myPacket.toCharArray(radiopacket,20);
  itoa(packetnum++, radiopacket+13, 10);
  //Serial.print("Sending "); Serial.println(radiopacket);
  radiopacket[19] = 0;
  rf95.send((uint8_t *)radiopacket, 20);


}

//void loop()
//{
//  sendPacket();
//}
