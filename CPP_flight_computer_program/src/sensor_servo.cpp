#include "Servo.h"
#include <Arduino.h>

Servo my_servo;  // Create a Servo object

bool deploy_parachute = true;  // State of the system
int servo_pin = 9;             //Pin to which the servo is connected
int position = 0;

//int val;  //  Varibale to read value from the sensor

void init_servo() {

  my_servo.attach(servo_pin);  // Attaches the servo connected at servo_pin to the my_sevo object
  my_servo.write(0);    //initializzes servo. Probably unnecessary if servo installed correctly?
}

void servo_180() {

  //  TURN SERVO 180 DEGREES WHEN SENSOR IS ACTIVATED
  if (deploy_parachute == true) {
    //for (position = 0; position <= 180; position++) { // goes from 0 to 180 
    for(position = 180; position >=0; position--) { // goes from 180 to 0
    
      my_servo.write(position);
      delay(5);
    }
    while (1);  //Stops servo
  }
}