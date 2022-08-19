#include "Tester.h"
int value = 0;
Tester Test1(5);
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("setup");
}

void loop() {
  
  Serial.println("Test C++");
  value = Test1.testCPlusPlus(4);
  Serial.println(value);
}
