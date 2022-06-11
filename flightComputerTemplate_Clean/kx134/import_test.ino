#include <Wire.h>
#include "SparkFun_Qwiic_KX13X.h"

void setup(){
  setupinit();
}

void loop(){
   
  float readX;
  float readY;
  float readZ;

  myData = kxAccel.getAccelData(); 

  readX = kx134_getXdata();
  readY = kx134_getYdata();
  readZ = kx134_getZdata();
  
  Serial.print("X: ");
  Serial.print(readX);
  Serial.print("g ");
  Serial.print(" Y: ");
  Serial.print(readY);
  Serial.print("g ");
  Serial.print(" Z: ");
  Serial.print(readZ);
  Serial.println("g ");
  
  delay(50); // Delay should be 1/ODR (Output Data Rate), default is 50Hz
}
