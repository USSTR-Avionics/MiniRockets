#include "Adafruit_FRAM_I2C.h"

int myInts[5];
int myIntsSize = 5;
int offsetfram = 0;
int fullBit = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

//  myInts[0] = 11;
//  myInts[1] = 1;
//  myInts[2] = 2;
//  myInts[3] = 3;
//  myInts[4] = 4;
  
  init_FRAM();
//  clearMem();
//  for (int i = 0; i<10000; i++)
//  {
//    if (fullBit != 1)
//    {
//      write_fram();
//    }
//    
//  }
  
  // 2. Read the value wrtten to location #0, Print it out, Write the value +1 to location #0
  // Read the first byte
  //uint8_t test = fram.read(0x0);
  //Serial.print("Restarted "); Serial.print(test); Serial.println(" times");
  // Test write ++
  //fram.write(0x7FE0, 299);


  // 3. Print out the value in every location
  
  read_frame();
  
}

void loop() {
  // put your main code here, to run repeatedly:
//test
}
