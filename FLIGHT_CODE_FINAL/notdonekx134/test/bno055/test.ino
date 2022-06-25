#include <Wire.h>





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initBNO055();
}

void loop() {
  printData();
  Serial.print(bno055_accel_x);
  Serial.print(",");
  Serial.print(bno055_accel_y);
  Serial.print(",");
  Serial.print(bno055_accel_z);
  Serial.print(",");
  Serial.print(bno055_orient_x);
  Serial.print(",");
  Serial.print(bno055_orient_y);
  Serial.print(",");
  Serial.print(bno055_orient_z);
  Serial.print(",");
  Serial.print(bno055_mag_x);
  Serial.print(",");
  Serial.print(bno055_mag_y);
  Serial.print(",");
  Serial.print(bno055_mag_z);
  Serial.print(",");
  Serial.print(bno055_gyro_x);
  Serial.print(",");
  Serial.print(bno055_gyro_y);
  Serial.print(",");
  Serial.println(bno055_gyro_z);
}
