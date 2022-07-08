#include "Tester.h"

// Measurement uncertainty
float r = 0;

// Estimate uncertainty
float p = 0;

int x = 2;

void setup() {
  // put your setup code here, to run once:
    int y = double_number(x);
    Serial.begin(9600);
    Serial.print(y);
 }

void loop() {
  // put your main code here, to run repeatedly:

}
