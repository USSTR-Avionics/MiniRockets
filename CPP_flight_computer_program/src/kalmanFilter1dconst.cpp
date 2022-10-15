/*
 * One Dimensional Constant Dynamic Kalman Filter
 * 
 * q (Process Noise Variance): The uncertainty in the measuremnt
 * k (Kalman gain)
 * p (Estimate Uncertainty): gets smaller with each iteration
 * r (Measurement Uncertainty)
 * x_ (Initial Guess): Imprecise intial guess should have high estimate error
 *    - estimate uncertainty = (estimate error)^2 ex. 100^2 = 10000
 * x_hat (Predicted Estimate): for constant dynamics the predicted estimate is equal to the current estimate
 * p_hat (Predicted Uncertainty Variance)
 * 
 * low measurement uncertainty -> high k -> new estimate close to measurement -> slow convergence of p
 * high measurement uncertainty -> low k -> new estimate close to previous estimate -> fast convergence of p
 * 
 * Algorithm:
 * 1. Initialization
 *    - x = inital guess
 *    - p = estimate uncertainty of intial guess
 *2. Prediction:
 *    - x_hat = x (our model has constant dynamics so predicted estimate = current estimate)
 *    - p_hat = p + q
 *1st Iteration
 *3. Measure:
 *    - The measurement value is the measurementValue paramater in the kalman filter function
 *    - Set the measurement uncertainty (r) based on the measurement error and variance
 * 4. Update
 *    - Kalman Gain: k = p_hat / (p_hat + r)
 *    - Current State Estimate:     x_ = x_ + k * (measurementValue - x_);
 *    - Current Estimate Uncertainty:     p = (1-k)*p_hat;
 * 
 */

#include "kalmanFilter1dconst.h"
// q = process noise variance
float q= 1;//0.15
// k = kalman gain
float k;
// p = estimate uncertainty
double p = 10000;//10000
float r = 10; // 0.01
// x_ = initial guess
float x_ = 518.5;
// z = measurement
//float z[10] = {50.45,50.967,51.6,52.106,52.492, 52.819,52.433,54.007,54.523,54.99};
// x_hat = predicted estimate
float x_hat = x_;
// p_hat = extrapolated estimate uncertainty
float p_hat = p+q;

float get_k(){
  return k;
}

float kalmanFilter(float measurementValue)
{
    // Current state estimates
    k = p_hat / (p_hat + r);
    x_ = x_ + k * (measurementValue - x_);
    p = (1-k)*p_hat;
    // Prediction
    x_hat=x_;
    p_hat=p+q;
    return x_hat;
}

//int main() {
//    std::cout << "Hello, World!" << std::endl;
//    for (int i = 0; i< 10; i++) {
//        std::cout << p_hat << std::endl;
//        kalmanFilter(i);
//    }
//    return 0;
//}
