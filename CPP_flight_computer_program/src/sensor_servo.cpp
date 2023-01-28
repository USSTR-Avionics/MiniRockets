#include "Servo.h"
#include <Arduino.h>
#include "sensor_servo.h"

Servo servo_obj;     // Create a Servo object
int servo_pin = SERVO_PIN;  //Pin to which the servo is connected
int position = SERVO_START_POSITION;

void init_servo()
	{
    servo_obj.attach(servo_pin);  
    servo_obj.write(position);    
	}

void servo_turn()
	//  TURN SERVO FROM 0 TO 180 DEGREES
	{
    for (position = 0; position <= 180; position++)
   		{    
      	servo_obj.write(position);
      	delay(1);
    	}
    exit(0);  //Stops servo
	}
