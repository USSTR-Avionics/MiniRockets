int double_number(int number);
float kalman_gain(float p, float r);
float state_update(float k, float x_hat_n_minus1, float z_n);
float covariance_update(float k, float p_n_minus1, float p_n);
float covariance_extrapolation(float p_n, float q_n);
