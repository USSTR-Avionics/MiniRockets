#include "Adafruit_FRAM_I2C.h"


/* Example code for the Adafruit I2C FRAM breakout */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5.0V DC
   Connect GROUND to common ground */
   
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();

float bno055_low()
  {
  return 0.00;   
  }

float bno055_high()
  {
  return 359.99;  
  }
  


void setup(void) 
  {
  Serial.begin(115200);
  
  if (fram.begin()) 
    {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C FRAM"); 
    } 
  else 
    {
    Serial.println("I2C FRAM not identified ... check your connections?\r\n");
    Serial.println("Will continue in case this processor doesn't support repeated start\r\n");
    while (1);
    }
  
  // Read the first byte
  uint8_t test = fram.read(0x0);
  Serial.print("Restarted "); Serial.print(test); Serial.println(" times");
  // Test write ++
  fram.write(0x0, test+1);

  int writeVar = 256;
  fram.write(0x100, writeVar); 

  uint8_t firstRead = fram.read(0x100);
  Serial.print("test read \r\n");
  Serial.print(firstRead);
  Serial.print("\r\n");
  Serial.println(bno055_low());
  Serial.println(bno055_high());
}

void loop(void) 
  {
  }
