#ifndef PACKAGE_STATISTICS_H
#define PACKAGE_STATISTICS_H

#define STRONG_EMA_SMOOTHING   0.1
#define MODERATE_EMA_SMOOTHING 0.5
#define WEAK_EMA_SMOOTHING     0.9

float get_exponential_moving_average(float new_value, float old_avg, float alpha_weight);

#endif // PACKAGE_STATISTICS_H
