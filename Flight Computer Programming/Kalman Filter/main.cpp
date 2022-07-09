#include <iostream>

int numCounter = 0 ;
int data[] = {1,2,3,4,5,6,7,8,9,10};
int data_size = 10;
float val0;
float val1;
float val2;
float val3;
int iterationCount;

float movingAverage(int i,float iterationCount) {

    if (numCounter==0)
    {
        val0 = data[i];
    }
    else if (numCounter==1)
    {
        val1 = data[i];
    }
    else if (numCounter==2)
    {
        val2 = data[i];
    }
    else
    {
        val3 = data[i];
    }

    if (numCounter<3)
    {
        numCounter+=1;
    }
    else
    {
        numCounter=0;
    }

    if (iterationCount >= 4)
    {
        return (val0+val1+val2+val3)/4;
    }
    else if (iterationCount == 3)
    {
        return (val0+val1+val2)/3;
    }
    else if (iterationCount == 2)
    {
        return (val0+val1)/2;
    }
    else
    {
        return val0;
    }


}

int main() {
    for (int i = 0; i < data_size; i++)
    {
        std::cout << numCounter << std::endl;
        iterationCount+=1;
        float ans = movingAverage(i, iterationCount);
        std::cout <<  "average: "<<ans << std::endl;
    }
    return 0;
}
