
//--------------LIBRARY------------------------
#include <Wire.h>
#include <MPU6050.h> //library from https://github.com/jarzebski/Arduino-MPU6050
//note for the MPU lib make sure that you have it named correctly and it is not conflicing with other possible libsPI
#include <SparkFunBME280.h> //library from https://github.com/sparkfun/SparkFun_BME280_Arduino_Library
//Note this is a port of the original PID LIB and still needs testing 
#include "OA_PID.h"
#include <Servo.h>
//----------------------------------------------
//--------------PRE-PROCESSOR VARIABLES---------
//this is where you will want to define your own pin numbers
//or if you dont have a specific component just delete or comment out the line
#define BUZZER 29 // 
#define LED1 33
#define LED2 32 // include as many LED etc as needed
#define SERVO_XPIN 3
#define SERVO_YPIN 5
#define MPU_ADDRESS 0x68
#define BMP_ADDRESS 0x76
#define LIFTOFF_THRESHOLD 1.15f // we suggest 1.15(measured in g's) but tweak this to your choosing!
#define X_SERVO_RATIO 1.0f //insert the servo to tvc ratio-- if you have 1:1 leave these as is 
#define Y_SERVO_RATIO 1.0f 
//THE ROCKET WILL NOT ENTER PAD IDLE IF DEBUG MODE IS ACTIVE 
//VVVVVVVVVVVVVVVVVVVVVVVVVVV
bool calibration_mode = false; //set this to true to use the serial monitor to find your x and y offset
//------------------------------------------------
//-----------VARIABLES----------------------------
//normally its bad to use so many global variables but this will simplfy our code
float aRes; //resolutions for the IMU
float gRes; //resolutions for the IMU
int16_t accelCount[3];
int16_t gyroCount[3];
float ax, ay, az;
float gx, gy, gz;
float yaw, pitch, roll;
float SelfTest[6];
float del_t = 0.0;
float pressure = 0.0;
float initial_altitude = 0.0;
float altitude = 0.0;
uint32_t last_update = 0;
uint32_t first_update = 0;
uint32_t current_update = 0;
int offsets[2];
double Outputx, Inputx, Outputy, Inputy;
double Setpointx, Setpointy;
float KPX = 1; // THESE PID VALUES WILL NEED TO BE CHANGED WITH YOUR OWN: the numbers chosen here are just to help make it obvious that the mount is moving the way you want
float KIX = .01;
float KDX = .01;
float KPY = 1;
float KIY = .01;
float  KDY = .01;
float PID_LIM = 15; // this is the output limit of our system
int decentCheck;
//-----------OBJECT DECLARATION-------------------
//MPU6050 mpu;
//BME280 bmp;
Servo x;
Servo y;
PID PIDx(&Inputx, &Outputx, &Setpointx, KPX, KIX, KDX, 1);
PID PIDy(&Inputx, &Outputx, &Setpointx, KPY, KIY, KDY, 1);
//--------------------------------------------------
//this is our State Machine-- this is where we wil store the current state of the rocket
//for all our functions to access
//adding new states is easy just use the format below and then to access them use rocket.yourStateHere
struct rocketState {
  bool padIdle = false;
  bool liftOff = false;
  bool apogee = false;
} rocket;
//-------------------------------------------------
void setup() {
  offsets[0] = 0; //THIS IS WHERE TO INSERT THE X OFFSET FROM THE CALIBRATION MODE
  offsets[1] = 0; //THIS IS WHERE TO INSERT THE Y OFFSET FROM THE CALIBRATION MODE
  //initialize everything
  initALL();
  if (calibration_mode) {
    Serial.println("Use W, A, S, D (press enter after each) to center your TVC mount");
    Serial.println("Press Z to exit");
    calibration();
    Serial.print("X AND Y OFFSETS ARE: ");
    Serial.print(offsets[0]);
    Serial.print(" | ");
    Serial.println(offsets[1]);
    Serial.println("WRITE THESE NUMBERS DOWN AND INSERT THEM IN THE CODE");
  }
  //initialize all the sensors and everything
}

void loop() {
  //keeping the loop or "main" function as simple and clear as possible is key
  padMode(rocket.padIdle); // this code is contstanly checking if it is time to call thse functions by checking the state machine
  tvcMode(rocket.liftOff);
  decentMode(rocket.apogee);
  dataReadout();
  
}
void calibration() {
  int X_offset = 0;
  int Y_offset = 0;
  int val = 0;
  while (val != 'z') {
    if (Serial.available()) {
      val = Serial.read();// then read the serial value
      if (val == 'd') {
        X_offset++;
      }
      if (val == 'a') {
        X_offset--;
      }
      if (val == 'w') {
        Y_offset++;
      }
      if (val == 's') {
        Y_offset--;
      }
      x.write(90 + X_offset);
      y.write(90 + Y_offset);
    }
  }
  offsets[0] = X_offset;
  offsets[1] = Y_offset;
}

void initALL() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  tone(BUZZER, 500);
  Wire.begin();
  Wire.setClock(4000000);
  Serial.begin(9600);
  x.attach(SERVO_XPIN);
  y.attach(SERVO_YPIN);

  //servo motion test
  x.write(50 + offsets[0]);
  y.write(50 + offsets[1]);
  delay(500);
  x.write(130 + offsets[0]);
  y.write(130 + offsets[1]);
  delay(500);
  x.write(90 + offsets[0]);
  y.write(90 + offsets[1]);

  
  if (!calibration_mode) {
    //initialize all our sensors
    if (initBMP() && initMPU() && initPID()) {
      Serial.println("Initialization Complete...");
      noTone(BUZZER);
        rocket.padIdle = true; 
    }
  }
}
//initialize the PID controller from the library
bool initPID() {
  PIDx.setSampleTime(10);
  PIDy.setSampleTime(10);
  PIDx.SetOutputLimits(-PID_LIM, PID_LIM);
  PIDy.SetOutputLimits(-PID_LIM, PID_LIM);
  Serial.println("PID Controller has been initialized...");
  return true;
}
//initialize bmp
bool initBMP() {
  bmp.setI2CAddress(BMP_ADDRESS);
  bmp.setPressureOverSample(16);//sets the bmp to sample at it's highest rate
  bmp.beginI2C();
  Serial.println("BMP has been initialized...");
  return true;
}
//this defines how we get the values from the bmp
void getBMP() {
  pressure = bmp.readFloatPressure();
  altitude = bmp.readFloatAltitudeMeters();
}
//initialize the mpu
bool initMPU() {
  if (mpu.begin(MPU6050_SCALE_250DPS, MPU6050_RANGE_4G)) {
    Serial.println("MPU has been initialized...");
    mpu.calibrateGyro();
    return true;
  }
}
//we could do this more optimally by using referencing to the mpu object, but this keeps things simple
// by seperating the get accel and get gyro we can optimize performance as we dont need to get both everytime
void getAccel() {
  Vector norm = mpu.readNormalizeAccel();
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
//at the very instant of lift off we will set these values to be our reference
void setPID() {
  for (int i = 0; i <= 2; i++) {
    Setpointx = pitch;
    Setpointy = yaw;
  }
}
//this converts our PID output to something our servos can use
void servoAction() {
  float sPitch, sYaw;
  sPitch = Outputx + 90 + offsets[0];
  sYaw = Outputy + 90 + offsets[1];
  x.write(sPitch);
  y.write(sYaw);
}
//this is where we define ourpad idle functionality
void padMode(bool state) {
  //here you can add LED lighting and buzzer functionality to indicate pad idle
  if (state) {
    getAccel();
    if (abs(az) > LIFTOFF_THRESHOLD) {
      rocket.liftOff = true;
      rocket.padIdle = false;
    }
  }
}
//this uses our pressure sensor to check for apogee
void apogeeCheck() {
  float last_alt = altitude;
  if (decentCheck > 10) {
    rocket.apogee = true;
    rocket.liftOff = false;
  }
  getBMP();
  if (last_alt - altitude > 2) {
    decentCheck++;
  }
}
//here we will want to use a timer to figure out when to deploy our chutes based on how long it has been since apogee
void decentMode(bool state) {
  if (state) {
    //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
    //youll want your chute deployment method in here
  }
}
//this simply integrates our gyro values into usable roll yaw and pitch
void resolveAngles() {
  //NOTE YOU MAY HAVE TO CHANGE THE LABELING BASED ON YOUR ORIENTATION
  yaw += gx * del_t;
  pitch += gy * del_t;
  roll += gz * del_t;
}
//this contains a lot of code and should really be broken up more
//but this is where we have our main flight software chunk the real meat of the code, what we are all here for
//this is the main flight loop
void tvcMode(bool state) {
  if (state) {
    getAccel();
    getGyro();
    current_update = micros(); // this grabs the current time in microseconds
    del_t = ((current_update - last_update) / 1000000.0f); //this is our time step for integration
    resolveAngles();
    last_update = current_update;
    Inputx = pitch; //again be ready to change these based on the orienttion of your IMU
    Inputy = yaw; //again be ready to change these based on the orienttion of your IMU
    setPID();
    PIDx.Compute();
    PIDy.Compute();
    servoAction();
    apogeeCheck();
  }
}
// this is a data read out for debug mode
void dataReadout() {
  Serial.print("ax,ay,az: ");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.print("\t");
  Serial.print("y,p,r: ");
  Serial.print(yaw);
  Serial.print("\t");
  Serial.print(pitch);
  Serial.print("\t");
  Serial.print(roll);
  Serial.print("\t");
  Serial.print("PID output(x is pitch| Y is yaw): ");
  Serial.print(Outputx);
  Serial.print("\t");
  Serial.print(Outputy);
  Serial.print("\t");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print("\t");
  Serial.print("Altitude (AGL): ");
  Serial.println(altitude - initial_altitude);
}
