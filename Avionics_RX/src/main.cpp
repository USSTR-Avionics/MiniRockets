#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "RFM95W.h"

void setup()
{
    Serial.begin(9600); // arg doesnt need to be 9600 just true
    Wire.begin();
}

void loop()
{
    // write your code here
}