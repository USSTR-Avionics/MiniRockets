
#include <iostream>
#include <math.h>
#include "BasicLinearAlgebra.h"

//#include "Tester.h"

// ESTIMATING THE TEMPERATURE OF THE LIQUID IN A TANK

// Process noise variance
double q = 0.15;

float z[10] = {50.45,50.967,51.6,52.106,52.492, 52.819,52.433,54.007,54.523,54.99};

// // ESTIMATE // //
// initial guess
float x = 10;
// estimate uncertainty
double p = pow(100,2);

// predicted estimate
float x_hat = 10;
// extrapolated estimate uncertainty (variance)
float p_hat = p+q;

float k;

int testering() {
    return 2;
}

float kalman_gain(float p, float r)
{
    std::cout << "P IS: " << p << std::endl;
    return p / (p + r);
}

float state_update(float k, float x_hat_n_minus1, float z_n)
{
    return x_hat_n_minus1 + k * (z_n - x_hat_n_minus1);
}

float covariance_update(float k, float p_n_minus1, float p_n)
{
    return (1 - k) * p_n_minus1;
}

float covariance_extrapolation(float p_n, float q_n)
{
    return (p_n + q_n);
}
/*
*
* This the Library for the Atlas Aerospace Kalman filter.
* This library may be modified to suit your specific use-case.
*
*
*	Written by:
*
*	Atlas Aerospace 03/08/2021
*
*
*	Some use cases may be:
*
*	- Filtering IMU data
*	- Filtering Barometer data
*	- Filtering Ultrasonic Distance sensors
*
*
*	My YouTube:
*
*	https://www.youtube.com/channel/UCWd6oqc8nbL-EX3Cxxk8wFA
*/

#pragma once
#include "BasicLinearAlgebra.h"
#include "math.h"

// Kalman Filter Struct
struct Kalman{

    // State space dynamics (EDIT TO MATCH YOUR STATE SPACE DYNAMICS)
    BLA::Matrix<2,2> A;
    BLA::Matrix<2> B;
    BLA::Matrix<2,2> Q;
    BLA::Matrix<1> R;
    BLA::Matrix<2> x;
    BLA::Matrix<1> u;
    BLA::Matrix<2,2> P = {0.0f, 0.0f, 0.0f, 0.0f};
    BLA::Matrix<2> K = {0.0f, 0.0f};
    BLA::Matrix<1,2> H = {0.0f, 1.0f};
    BLA::Matrix<2,2> I = {0.0f, 0.0f, 0.0f, 0.0f};
    BLA::Matrix<2> xPredicted;
    BLA::Matrix<2,2> pMinus;
    BLA::Matrix<1> S;

    // Filter
    void updateState(BLA::Matrix<1> y, BLA::Matrix<1> u, float dt){

        // Update the Kalman filter
        xPredicted = A * x + B * u;
        xPredicted = x + xPredicted * dt;
        pMinus = A * P * ~A + Q;

        // Correct the Kalman filter error
        S = H * pMinus * ~H + R;
        //this-> K = pMinus * ~H * BLA::Invert(S);
        this-> x = xPredicted + K * (y - H * xPredicted);
        this-> P = (I - K * H) * pMinus;
    }
};
int main() {
    //Matrix<2, 2> B{0.0, 45.34, 32.98, 1456.1222};
    std::cout << q<< std::endl;
    std::cout << p_hat << std::endl;
    for (int i = 0; i< 10; i++) {
        std::cout << "ITERATION " << i+1 << std::endl;
        // Current State Estimates
        k = kalman_gain(p_hat,0.01);
        std::cout << "K IS: " << k << std::endl;
        x = state_update(k,x,z[i]);
        std::cout << "x (current state estimate) IS: " << x << std::endl;
        p = (1-k)*p_hat;
        std::cout << "p (current state estimate uncertainty is): " << p << std::endl;
        BLA::Matrix<2,2> A;

        std::cout << k << std::endl;
        // Prediction
        x_hat = x;
        p_hat=p+q;
        std::cout << "x_hat "<< x_hat << std::endl;
        std::cout << "p_hat "<< p_hat << std::endl;
        //std::cout << p_hat << std::endl;
    }

    return 0;
}

