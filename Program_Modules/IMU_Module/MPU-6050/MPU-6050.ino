#include <MPU6050.h> //library

#define MPU_ADDRESS 0x68

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

MPU6050 mpu;



//initialize the mpu
bool initMPU() {
  if (mpu.begin(MPU6050_SCALE_250DPS, MPU6050_RANGE_4G)) {
    //Serial.println("MPU has been initialized...");
    mpu.calibrateGyro();
    return true;
  }
}

void getAccel() {
  Vector norm = mpu.readNormalizeAccel();
  //Serial.print("w");
  ax = norm.XAxis;
  ay = norm.YAxis;
  az = norm.ZAxis;
}

void getGyro() {
  Vector norm = mpu.readNormalizeGyro();
  gx = norm.XAxis;
  gy = norm.YAxis;
  gz = norm.ZAxis;
}

void getTemp() {
  temp = mpu.readTemperature();
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("TEST");
  initMPU();
  mpu.setAccelOffsetX(-1364);
  mpu.setAccelOffsetY(-867);
  mpu.setAccelOffsetZ(897);
  mpu.setGyroOffsetX(146);
  mpu.setGyroOffsetY(4);
  mpu.setGyroOffsetZ(150);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  tvcMode(true);
  // Sensor Interface Display Code
  //char text[100];
  //sprintf(text,"%f,%f,%f,%f,%f,%f",ax,ay,az,yaw,pitch,roll);
  //Serial.print(20);
  //Serial.print(",");
  Serial.print(ax);
  Serial.print(",");
  Serial.print(ay);
  Serial.print(",");
  Serial.print(az);
  Serial.print(",");
  Serial.print(gx);
  Serial.print(",");
  Serial.print(gy);
  Serial.print(",");
  Serial.print(gz);
  Serial.print(",");
  Serial.print(yaw);
  Serial.print(",");
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.print(current_update);
  Serial.print(",");
  Serial.print(last_update);
  Serial.print(",");
  Serial.print(del_t);
  Serial.println();
  // Delay 10 ms -> approximately 100 samples/sec ASSUMING there is no blocking code
  delay(10);
}
