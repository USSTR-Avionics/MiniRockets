#include <Arduino.h>

#define GREEN 5
#define BLUE 4
#define RED 3


int init_LED()  
    {
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