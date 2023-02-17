#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RFM95W.h"

uint16_t Slave{13};
uint16_t Interrupt{32};
uint16_t Reset{30};

static RFM95W Radio(Slave, Interrupt);

void setup()
{
    pinMode(Reset, OUTPUT);
    digitalWrite(Reset, HIGH);

    while (!Serial);
    Serial.begin(9600);
    delay(100);
    Wire.begin();

    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    
    // delete this if compile fail
    Radio.check();

    // do the reset for the radio
    Radio.Init();
    Radio.Set_Frequency(914.1);
}

void loop()
{
    delay(1000);
    Radio.UDP_Send("Hello from the other side");
    Serial.println("loop");

    // write your code here
}
