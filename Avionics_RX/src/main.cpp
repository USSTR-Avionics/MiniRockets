#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RFM95W.h"

uint16_t CS{10};
uint16_t Interrupt{32};
uint16_t Reset{30};

RFM95W RF95;

void setup()
{
    while (!Serial);
    Serial.begin(9600);
    delay(100);
    Wire.begin();

    RH_RF95 Radio(CS, Interrupt);

    pinMode(Reset, OUTPUT);
    digitalWrite(Reset, HIGH);
    // manual reset
    digitalWrite(Reset, LOW);
    delay(10);
    digitalWrite(Reset, HIGH);
    delay(10);
    // =======================  NO TOUCH =======================
    // ?? huh? why did this fix the bug????
    Radio.init();
    RF95.Init(Radio);
    // ===================================================
    // do the reset for the radio
    RF95.Set_Frequency(915.7);

}

void loop()
{
    delay(1000);
    Serial.println(RF95.Mem_check());
    RF95.UDP_Send("Hello from the other side");

}
