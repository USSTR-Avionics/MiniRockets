

/* Example code for the Adafruit I2C FRAM breakout */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5.0V DC
   Connect GROUND to common ground */
   
Adafruit_FRAM_I2C fram     = Adafruit_FRAM_I2C();

bool init_FRAM(){
    // 1. Verify the chip has been found
  if (fram.begin()) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C FRAM");
    return true;
  } else {
    Serial.println("I2C FRAM not identified ... check your connections?\r\n");
    Serial.println("Will continue in case this processor doesn't support repeated start\r\n");
    return false;
  }
}

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
  // 32768
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

void write_fram()
{
  for (int i = 0; i < myIntsSize; i++) {
  //Serial.println(i);
  if ( (i+offsetfram)<32767)
  {
    fram.write(i+offsetfram, myInts[i]);
  }
  else
  {
    fram.write(i+offsetfram, 9);
    fullBit = 1;
    Serial.println("FRAM FULL");
    return;
  }
  }
  offsetfram=offsetfram+myIntsSize;
  
}




//void setup(void) {
//  
//  
//  
//}
//
//void loop(void) {
//
//}
