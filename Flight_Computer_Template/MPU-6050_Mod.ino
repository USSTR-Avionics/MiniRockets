//#include <MPU6050.h> //library

#define MPU_ADDRESS 0x68
#include "globalvars.h"
float aRes; //resolutions for the IMU
float gRes; //resolutions for the IMU
int16_t accelCount[3];
int16_t gyroCount[3];
float ax, ay, az;
float gx, gy, gz;
float yaw, pitch, roll;
float del_t = 0.0;
float temp = 0.0;
uint32_t current_update = 0;
uint32_t last_update = 0;

//MPU6050 mpu;



//initialize the mpu
bool initMPU() {
//  if (mpu.begin(MPU6050_SCALE_250DPS, MPU6050_RANGE_4G)) {
    //Serial.println("MPU has been initialized...");
    mpu.calibrateGyro();
    mpu.setAccelOffsetX(-2375);
    mpu.setAccelOffsetY(-2233);
    mpu.setAccelOffsetZ(3324);
    mpu.setGyroOffsetX(146);
    mpu.setGyroOffsetY(3);
    mpu.setGyroOffsetZ(151);
    return true;
  }
}

void getAccel() {
  Vector norm = mpu.readNormalizeAccel();
  //Serial.print("w");
  ax = norm.XAxis;
  ay = norm.YAxis;
  az = norm.ZAxis;
  //Serial.print("Ax");
  axglob = norm.XAxis;
  ayglob = norm.YAxis;
  azglob = norm.ZAxis;
}


void getGyro() {
  Vector norm = mpu.readNormalizeGyro();
  gxglob = norm.XAxis;
  gyglob = norm.YAxis;
  gzglob = norm.ZAxis;
}

void getTemp() {
//  temp = mpu.readTemperature();
}

//this simply integrates our gyro values into usable roll yaw and pitch
void resolveAngles() {
  //NOTE YOU MAY HAVE TO CHANGE THE LABELING BASED ON YOUR ORIENTATION

  yaw += gx * del_t;
  pitch += gy * del_t;
  roll += gz * del_t;
}

void tvcMode(bool state) {
  
  if (state) {
    
    getAccel();
    getGyro();
    getTemp();
    current_update = micros(); // this grabs the current time in microseconds
    del_t = ((current_update - last_update) / 1000000.0f); //this is our time step for integration

resolveAngles();
    last_update = current_update;
    //Inputx = pitch; //again be ready to change these based on the orienttion of your IMU
    //Inputy = yaw; //again be ready to change these based on the orienttion of your IMU

  }
}

/*void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  initMPU();
  
}*/

/*void loop() {
  
  // put your main code here, to run repeatedly:
  //tvcMode(true);
  // Sensor Interface Display Code
  //char text[100];
  //sprintf(text,"%f,%f,%f,%f,%f,%f",ax,ay,az,yaw,pitch,roll);
  //Serial.print(20);
  //Serial.print(",");
  
  // Delay 10 ms -> approximately 100 samples/sec ASSUMING there is no blocking code
  delay(10);
}*/
