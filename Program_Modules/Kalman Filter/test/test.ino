#include "Tester.h"
#include "movingAverage.h"

// Measurement uncertainty
float r = 0;

// Estimate uncertainty
float p = 0;

int x = 2;

/////
int iterationCount;
float val0;
float val1;
float val2;
float val3;
int numCounter = 0 ;
float currentVal = 0;


void updateNumCount()
{
  
    if (numCounter==0)
    {
        val0 = currentVal;
    }
    else if (numCounter==1)
    {
        val1 = currentVal;
    }
    else if (numCounter==2)
    {
        val2 = currentVal;
    }
    else
    {
        val3 = currentVal;
    }

    if (numCounter<3)
    {
        numCounter+=1;
    }
    else
    {
        numCounter=0;
    }
}

void setup() {
  // put your setup code here, to run once:
    int y = double_number(x);
    Serial.begin(9600);
    Serial.print(y);
    float xdd = movingAverage(2.34,1,1,2,3,4);

 }

void loop() {
  // put your main code here, to run repeatedly:

}
