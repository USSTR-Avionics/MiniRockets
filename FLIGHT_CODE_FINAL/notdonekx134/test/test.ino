#include <Wire.h>
#include "SparkFun_Qwiic_KX13X.h"

//QwiicKX132 kxAccel;
QwiicKX134 kxAccel; // Uncomment this if using the KX134 - check your board
                      //if unsure.  
outputData kx134_accel; // This will hold the accelerometer's output. 
float kx134_accel_x;
float kx134_accel_y;
float kx134_accel_z;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  initKX134();
}

void loop() {
  getKX134_Accel();
  Serial.print(kx134_accel_x);
  Serial.print(",");
  Serial.print(kx134_accel_y);
  Serial.print(",");
  Serial.println(kx134_accel_z);
}
