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
 * 2. Connect BLUE to 10 ohm resistor, GREEM to a 10 ohm resistor, and RED to a 100 ohm resistor
 * 3. Connect BLUE to pin 38, GREEN to pin 39, RED to pin 40, and CATHODE to GND
 * 
 * Functions:
 * ledON(color): changes color to "BLUE", "GREEN",OR "RED" based on color parameter
 * ledOFF: turns the LED off
 */

// Define Pins
  #define BLUE 38
  #define GREEN 39
  #define RED 40

void setup() {
  LED_initSensor();
}



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
}

void LED_initSensor() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void ledON(String color) {
  if (color == "RED") {
    digitalWrite(RED,HIGH);
    digitalWrite(GREEN,LOW);
    digitalWrite(BLUE,LOW);
  }
  if (color == "GREEN") {
    digitalWrite(RED,LOW);
    digitalWrite(GREEN,HIGH);
    digitalWrite(BLUE,LOW);
  }
  if (color == "BLUE") {
    digitalWrite(RED,0);
    digitalWrite(BLUE,HIGH);
    digitalWrite(GREEN,LOW);
  }
  
}

void ledOFF() {
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,LOW);
}
