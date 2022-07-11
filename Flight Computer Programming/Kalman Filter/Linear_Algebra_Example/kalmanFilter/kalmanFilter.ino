#include <BasicLinearAlgebra.h>
#include "Filter.h"


using namespace BLA;


void setup()
{
    Serial.begin(115200);

    

    // Now you can do some matrix math! The Matrix class supports addition and subtraction between matrices of the same
    // size:
    BLA::Matrix<3, 3> J;

    float altitude[30] = {-32.4,-11.1,18,22.9,19.5,28.5,46.5,68.9,48.2,56.1,90.5,104.9,140.9,148,187.6,209.2,244.6,276.4,323.5,357.3,357.4,398.3,446.7,465.1,529.4,570.4,636.8,693.3,707.3,748.5};
    float acceleration[30] = {39.72, 40.02,39.97,39.81,39.75,39.6,39.77,39.83,39.73,39.87,39.81,39.92,39.78,39.98,39.61,39.86,39.74,39.87,39.63,39.67,39.96,39.8,39.89,39.85,39.9,39.81,39.68};
    

    // An inverse of a matrix can also be calculated for square matrices via the Invert function:
    BLA::Matrix<3, 3> J_inv = J;
    BLA::Matrix<1,1> sss = 1;
    BLA::Matrix<1,1> ddd = 2;
    bool is_nonsingular = Invert(J_inv);
    for (int i = 0; i<2; i++)
    {
      iterate(altitude[i],acceleration[i]);
    Serial << "x " << x << '\n';
    Serial << "F " << F << '\n';
    Serial << "G " << G << '\n';
    Serial << "Q " << Q << '\n';
    Serial << "R " << R_n << '\n';
    Serial << "P " << P << '\n';
    Serial << "H " << H << '\n';
    Serial << "x_hat " << x_hat << '\n';
    Serial << "midstep " << midstep << '\n';
    Serial << "midstep_invert " << midstep_invert_val << '\n';
    Serial << "k " << k << '\n';
    Serial << "p_hat " << p_hat << '\n';
    }
    
}

void loop() {}
