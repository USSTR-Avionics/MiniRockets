#include "Kalman.h"


KalmanFilter::KalmanFilter(float Q, float R) {
  Q = Q;
  R = R;
}

// positive direction corresponds to more dependency on dynamics
void KalmanFilter::changeQR(int direction) {
  if (direction > 0) {
    for(auto i = 0; i < direction; i++){
    Q /= 1.1f;
    }
  } else if (direction < 0) {
        for(auto i = 0; i < direction; i++){
    Q *= 1.1f;
    }
  }
}

float KalmanFilter::update(float U, float Y) {
  float x_hat_new = A * x_hat + B * U;
  P = A * P * A + Q;
  float K = P * C * (1.0f / (C * P * C + R));
  x_hat_new += K * (Y - C * x_hat_new);
  P = (1 - K * C) * P;
  x_hat = x_hat_new;
  return x_hat;
}

//  ~source algorithm~
//  x_hat_new = A * x_hat;
//  P = A*P*A.transpose() + Q;
//  K = P*C.transpose()*(C*P*C.transpose() + R).inverse();
//  x_hat_new += K * (y - C*x_hat_new);
//  P = (I - K*C)*P;
//  x_hat = x_hat_new;
