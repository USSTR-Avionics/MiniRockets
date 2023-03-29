#ifndef PACKAGE_STATISTICS_H
#define PACKAGE_STATISTICS_H

#define ZERO_FLOAT             0.0f

#define M_PI 3.1415926535897932384626433832795

#define STRONG_EMA_SMOOTHING   0.1f
#define MODERATE_EMA_SMOOTHING 0.5f
#define WEAK_EMA_SMOOTHING     0.9f

float get_exponential_moving_average(float new_value, float old_avg, float alpha_weight);

#endif // PACKAGE_STATISTICS_H
