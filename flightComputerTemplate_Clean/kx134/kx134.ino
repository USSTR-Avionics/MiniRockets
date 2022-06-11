#include <Wire.h>
#include "SparkFun_Qwiic_KX13X.h"
//QwiicKX132 kxAccel;
QwiicKX134 kxAccel; // Uncomment this if using the KX134 - check your board
                      //if unsure.  
outputData myData; // This will hold the accelerometer's output. 

void setupinit() {
  
  while(!Serial){
    delay(50);
  }

  Serial.begin(115200);
  Serial.println("Welcome.");

  Wire.begin();
  if( !kxAccel.begin() ){
    Serial.println("Could not communicate with the the KX13X. Freezing.");
    while(1);
  }
  else
    Serial.println("Ready.");
    
  if( !kxAccel.initialize(DEFAULT_SETTINGS)){ // Loading default settings.
    Serial.println("Could not initialize the chip.");
    while(1);
  }
  else
    Serial.println("Initialized...");

  // kxAccel.setRange(KX132_RANGE16G);
  // kxAccel.setRange(KX134_RANGE32G); // For a larger range uncomment

}

float kx134_getXdata(){
  return myData.xData;  
}

float kx134_getYdata(){
  return myData.yData;  
}

float kx134_getZdata(){
  return myData.zData;  
}
  
//void loop() {
//
//  float readX;
//  float readY;
//  float readZ;
//
//  myData = kxAccel.getAccelData(); 
//  
//  readX = kx134_getXdata();
//  readY = kx134_getYdata();
//  readZ = kx134_getZdata();
//  
//  Serial.print("X: ");
//  Serial.print(readX);
//  Serial.print("g ");
//  Serial.print(" Y: ");
//  Serial.print(readY);
//  Serial.print("g ");
//  Serial.print(" Z: ");
//  Serial.print(readZ);
//  Serial.println("g ");
//  
//  delay(250); // Delay should be 1/ODR (Output Data Rate), default is 50Hz
//
//}
