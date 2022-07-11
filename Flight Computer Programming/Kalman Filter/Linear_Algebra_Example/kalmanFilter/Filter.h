#pragma once
#include <cmath>
    BLA::Matrix<2,1> x_hat;
    // Set a very high estimate uncertainty for intial guess 
    BLA::Matrix<2,2> P = {500,0,0,500};
    BLA::Matrix<2,2> p_hat;
    BLA::Matrix<1,1> midstep;
    BLA::Matrix<1,1> midstep_invert;
    BLA::Matrix<2,1> k;
    float midstep_invert_val;
    BLA::Matrix<1,1> current_state_midstep;
    BLA::Matrix<2,2> identity = {1,0,0,1};
    
    // We do not know the rocket location, so set initial position (x_n) and velocity (x_n_dot) to 0
    float x_n = 0;
    float x_n_dot = 0;
    BLA::Matrix<2,1> x = {x_n, x_n_dot};
    
    // For this example: t = 0.25 s
    float delta_t = 0.25;

    
    // F = state transition matrix
    BLA::Matrix<2,2> F = {1,delta_t,0,1};
    
    // G = control matrix
    BLA::Matrix<2,1> G = {0.5*(pow(delta_t,2)),delta_t};
    
    // assume rocket acceleration is 9,8 for first
    float un= 9.8;
    
    //x_n_MAT= {x_n, x_n_dot};

    // Covariance Extrapolation Equation
    float random_variance = 1.1*1.1;
    // p = estimate uncertainty
    //BLA::Matrix<2,2> P_n = {1.1,1.1,1.1,1.1};
    BLA::Matrix<2,2> Q = {pow(delta_t,4)/4,pow(delta_t,3)/2,pow(delta_t,3)/2,pow(delta_t,2)};
    
    
    // Measurement Equation
    BLA::Matrix<1,2> H = {1,0};
    BLA::Matrix<1,1> z_n = H*x;
    
    // For simplicity, assuming costant measurement uncertainty for measurement covariance matrix R
    // Altimeter measurement error standard deiation = 20m
    BLA::Matrix<1,1> R_n=20*20;

    // Kalman Gain
    //BLA::Matrix<2,1> k = P_n_minus1*(~H)*Invert(H*P_n_minus1*(~H)+R_n);
    //BLA::Matrix<2,2> test = Invert(P_n_plus1);


    // State Update Equation
    
    // Filter
    void iterate(float z, float u) {
        BLA::Matrix<1,1> zz = {z};
        BLA::Matrix<1,1> uu = {u-9.8};
        x_hat = F*x + G*un;
        p_hat = F*P*(~F)+Q;
        // Update the Kalman filter
        midstep = (H*p_hat*(~H)+R_n);
        midstep_invert_val=1/midstep(0,0);
        midstep_invert = {midstep_invert_val};
        k = p_hat*(~H)*midstep_invert;
        x = x_hat + k*(zz-(H*x_hat));
        P = (identity-k*H)*p_hat*(~(identity-k*H))+ k*R_n*(~k);
        // Predict
        x_hat = F*x+G*uu;
        p_hat = F*P*(~F)+Q;
        
    }
//};
