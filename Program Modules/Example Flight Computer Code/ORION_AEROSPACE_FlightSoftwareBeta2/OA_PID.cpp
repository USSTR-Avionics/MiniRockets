#include "OA_PID.h"

PID::PID(double* Input, double* Output, double* Setpoint,float Kp,float Ki,float Kd,int ControllerDirection) {
  myOutput = Output;
  myInput = Input;
  mySetpoint = Setpoint;

  PID::SetOutputLimits(0, 255);
  SampleTime = 100;       //default Controller Sample Time is 0.1 seconds
  SetControllerDirection(ControllerDirection);
  SetTunings(Kp, Ki, Kd);
  lastTime = millis() - SampleTime;

}
bool PID::Compute()
{
  unsigned long now = millis();
  unsigned long timeChange = (now - lastTime);
  if (timeChange >= SampleTime)
  {
    /*Compute all the working error variables*/
    double input = *myInput;
    double error = *mySetpoint - input;
    ITerm += (ki * error);
    if (ITerm > outMax) ITerm = outMax;
    else if (ITerm < outMin) ITerm = outMin;
    double dInput = (input - lastInput);

    /*Compute PID Output*/
    double output = kp * error + ITerm - kd * dInput;

    if (output > outMax) output = outMax;
    else if (output < outMin) output = outMin;
    myOutput = &output;

    /*Remember some variables for next time*/
    lastInput = input;
    lastTime = now;
    return true;
  }
  else return false;
}
void PID::setSampleTime(int NewSampleTime) {
  SampleTime = (unsigned long) NewSampleTime;
}

void PID::SetOutputLimits(double Min, double Max)
{
  if (Min >= Max) return;
  outMin = Min;
  outMax = Max;
  double myOutVal = *myOutput;
  if (inAuto)
  {
    if (myOutVal > outMax) myOutVal = outMax;
    else if (myOutVal < outMin) myOutVal = outMin;

    if (ITerm > outMax) ITerm = outMax;
    else if (ITerm < outMin) ITerm = outMin;
  }
}
void PID::SetTunings(double Kp, double Ki, double Kd)
{
  if (Kp < 0 || Ki < 0 || Kd < 0) return;


  double SampleTimeInSec = ((double)SampleTime) / 1000;
  kp = Kp;
  ki = Ki * SampleTimeInSec;
  kd = Kd / SampleTimeInSec;

  if (controllerDirection == REVERSE)
  {
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
}
void PID::SetControllerDirection(int Direction)
{
  if (Direction != controllerDirection)
  {
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
  controllerDirection = Direction;
}
