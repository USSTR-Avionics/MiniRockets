#include <Arduino.h>

#define BUILTIN_LED 13
#define GREEN 5
#define BLUE 4
#define RED 3


int init_LED()  
    {
    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(RED, OUTPUT);
    return EXIT_SUCCESS;
    }

void setLedGreen()
    {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    }

void setLedRed()
    {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    }

void setLedBlue()
    {
    digitalWrite(BLUE, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    }

void flashSOS()
    {
    while (true)
        {
        digitalWrite(BLUE, HIGH);
        delay(500);
        digitalWrite(BLUE, LOW);
        }
    }

void flashInternalLed(bool x)
    {
    if (x == true)
        {
        digitalWrite(BUILTIN_LED, HIGH);
        }
    else
        {
        digitalWrite(BUILTIN_LED, LOW);
        }
    }