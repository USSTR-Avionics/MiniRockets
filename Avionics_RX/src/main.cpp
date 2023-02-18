#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RFM95W.h"
#include <RH_RF95.h>

uint16_t Slave{13};
uint16_t Interrupt{32};
uint16_t Reset{30};

// it got stuck here, try commenting out switch mode, it works
static RFM95W Radio(Slave, Interrupt);

void setup()
{
    Serial.begin(9600); // arg doesn't need to be 9600 just true
    Wire.begin();
    
    // // delete this if compile fail
    Radio.check();

    // do the reset for the radio
    // Radio.Init();
    Radio.Set_Frequency(914.1);
}

void loop()
{
    delay(1000);
    // Radio.UDP_Send("Hello from the other side");
    Serial.println("loop");

    // write your code here
}
