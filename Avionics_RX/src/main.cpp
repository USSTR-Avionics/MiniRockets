#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RFM95W.h"
#include <RH_RF95.h>

static RFM95W Radio(13, 32, 30);


void setup()
{
    Serial.begin(9600); // arg doesn't need to be 9600 just true
    Wire.begin();
    Radio.Set_Frequency(914.1);
}

void loop()
{
    delay(1000);
    Radio.UDP_Send("Hello from the other side");
    Serial.println("loop");

    // write your code here
}