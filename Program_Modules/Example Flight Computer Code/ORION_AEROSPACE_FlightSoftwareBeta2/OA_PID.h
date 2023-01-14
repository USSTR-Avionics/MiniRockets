
#pragma once
#include <Wire.h>
#include <Servo.h>
#include <arduino-timer.h>
#define AUTOMATIC   1
#define MANUAL      0
#define DIRECT      0
#define REVERSE     1

//a port of the br3ttb.pid.esp8266-nonos.cpp lib 
class PID {
  public:
  PID(double* Input, double* Output, double* Setpoint,float Kp,float Ki,float Kd,int ControllerDirection);
  void setSampleTime(int NewSampleTime);
  void SetOutputLimits(double Min, double Max);
  void SetControllerDirection(int Direction);
  void SetTunings(double Kp, double Ki, double Kd);
  bool Compute();
  private:
                     
    double kp;                          
    double ki;                         
    double kd;                          

    int controllerDirection;

    double *myInput;                    
    double *myOutput;                   
    double *mySetpoint;                 

    unsigned long lastTime;
    double ITerm, lastInput;

    unsigned long SampleTime;
    double outMin, outMax;
    bool inAuto;
};
