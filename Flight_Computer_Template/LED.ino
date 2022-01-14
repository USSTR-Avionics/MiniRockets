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
  #define BLUE 13
  #define GREEN 14
  #define RED 15

/*void setup() {
  
  initSensor();
}
*/

/*
void loop() {
  delay(1000);
  ledON("RED");
  delay(1000);
  ledOFF();
  delay(1000);
  ledON("GREEN");
  delay(1000);
  ledOFF();
  delay(1000);
  ledON("BLUE");
  delay(1000);
  ledOFF();
  delay(1000);
}*/

void LED_initSensor() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void ledON(String color) {
  if (color == "RED") {
    analogWrite(RED,255);
    analogWrite(GREEN,0);
    analogWrite(BLUE,0);
  }
  if (color == "GREEN") {
    analogWrite(RED,0);
    analogWrite(GREEN,255);
    analogWrite(BLUE,0);
  }
  if (color == "BLUE") {
    analogWrite(RED,0);
    analogWrite(GREEN,0);
    analogWrite(BLUE,255);
  }
  
}

void ledOFF() {
  analogWrite(RED,0);
  analogWrite(GREEN,0);
  analogWrite(BLUE,0);
}
