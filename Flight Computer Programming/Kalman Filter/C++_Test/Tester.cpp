#include "Tester.h"

// 1. The state update equations.
// 2. The dynamic model equations.
// 3. The Kalman Gain equation.
// 4. The Covariance Update equation
// 5. The Covariance Extrapolation equation

int myNumber;
int double_number(int number)
{
	return number * 2;
}

/*
	Uncertainty in Estimate / (Uncertainty in Estimate + Uncertainty in Measurement)
	A number between 0 and 1
*/
float kalman_gain(float p, float r)
{
	return p / (p + r);
}

/* 
	k: kalman gain
	z_n: measurement at time n
	x_hat_n: estimate of x at time n
	x_hat_n_minus1: previous estimate of x at n-1
	x_hat_n_plus1: estimate of future state at n+1
*/
float state_update(float k, float x_hat_n_minus1, float z_n)
{
	return x_hat_n_minus1 + k * (z_n - x_hat_n_minus1);
}

/*
	k: kalman gain
	p_n_minus1: previous estimate uncertainty
	p_n: current estimate uncertainty
*/
float covariance_update(float k, float p_n_minus1, float p_n)
{
	return (1 - k) * p_n_minus1;
}

float covariance_extrapolation(float p_n, float q_n)
{
	return (p_n + q_n);
}
