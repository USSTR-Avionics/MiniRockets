#include <iostream>
#include <math.h>

// q = process noise variance
float q= 0.15;
// k = kalman gain
float k;
// p = estimate uncertainty
double p = pow(100,2);;
float r = 0.01;
// x = initial guess
float x = 10;
// z = measurement
float z[10] = {50.45,50.967,51.6,52.106,52.492, 52.819,52.433,54.007,54.523,54.99};
// x_hat = predicted estimate
float x_hat = 10;
// p_hat = extrapolated estimate uncertainty
float p_hat = p+q;

void kalmanFilter(int i)
{
    std::cout << "p_hat IS: " << p_hat << std::endl;
    std::cout << "r IS: " << r << std::endl;
    // Current state estimates
    k = p_hat / (p_hat + r);
    std::cout << "K IS: " << k << std::endl;
    x = x + k * (z[i] - x);
    std::cout << "x (current state estimate) IS: " << x << std::endl;
    p = (1-k)*p_hat;
    std::cout << "p (current state estimate uncertainty is): " << p << std::endl;
    // Prediction
    x_hat=x;
    p_hat=p+q;
    std::cout << "x_hat "<< x_hat << std::endl;
    std::cout << "p_hat "<< p_hat << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    for (int i = 0; i< 10; i++) {
        std::cout << p_hat << std::endl;
        kalmanFilter(i);
    }
    return 0;
}
