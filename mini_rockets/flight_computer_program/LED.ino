/*
 * LED Module
 * 
 * Inputs: BLUE, GREEN, CATHODE, RED
 * 
 * Outputs:
 * - red light
 * - green light
 * - blue light
 * 
 * Set Up Instructions: 
 * 1. LED input pin order is BLUE, GREEN, CATHODE, RED with CATHODE having a longer wire
 * 2. Connect BLUE to 220 ohm resistor, GREEM to a 220 ohm resistor, and RED to a 220 ohm resistor
 * 3. Connect BLUE to pin 3 (PIN 13 ON TEENSY), GREEN to pin 5 (PIN 14 ON TEENSY), RED to pin 6 (PIN 15 ON TEENSY), and CATHODE to GND
 
 * Functions:
 * ledON(color): changes color to "BLUE", "GREEN",OR "RED" based on color parameter
 * ledOFF: turns the LED off
 */

// Define Pins
  #define BLUE 38
  #define GREEN 39
  #define RED 40

void LED_initSensor() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void ledON(String color) {
  //Serial.println(color);
  if (color == "RED") {
    digitalWrite(RED,1);
    digitalWrite(GREEN,0);
    digitalWrite(BLUE,0);
  }
  if (color == "GREEN") {
    analogWrite(RED,0);
    digitalWrite(GREEN,1);
    analogWrite(BLUE,0);
  }
  if (color == "BLUE") {
    digitalWrite(RED,0);
    digitalWrite(GREEN,0);
    digitalWrite(BLUE,1);
  }
  
}

void ledOFF() {
  analogWrite(RED,0);
  analogWrite(GREEN,0);
  analogWrite(BLUE,0);
}
