#include "Adafruit_FRAM_I2C.h"

/* Example code for the Adafruit I2C FRAM breakout */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5.0V DC
   Connect GROUND to common ground */
   
Adafruit_FRAM_I2C fram     = Adafruit_FRAM_I2C();

void clearMem(){
    // dump the entire 32K of memory!
  uint8_t value;
  for (uint16_t a = 0; a < 32768; a++) {
    fram.write(a, 0);
    Serial.print("CLEARED ");
    Serial.println(a);
  }
  
}

void read_frame(){
    // dump the entire 32K of memory!
  uint8_t value;
  for (uint16_t a = 0; a < 32768; a++) {
    value = fram.read(a);
    if ((a % 32) == 0) {
      Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
    }
    Serial.print("0x"); 
    if (value < 0x1) 
      Serial.print('0');
    Serial.print(value, HEX); Serial.print(" ");
  }
  
}




void setup(void) {
  Serial.begin(9600);


  // 1. Verify the chip has been found
  
  if (fram.begin()) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C FRAM");
  } else {
    Serial.println("I2C FRAM not identified ... check your connections?\r\n");
    Serial.println("Will continue in case this processor doesn't support repeated start\r\n");
    while (1);
  }

  clearMem();
  // 2. Read the value wrtten to location #0, Print it out, Write the value +1 to location #0
  
  // Read the first byte
  uint8_t test = fram.read(0x0);
  Serial.print("Restarted "); Serial.print(test); Serial.println(" times");
  // Test write ++
  fram.write(0x7FE0, 299);


  // 3. Print out the value in every location
  
  read_frame();
  
}

void loop(void) {

}
