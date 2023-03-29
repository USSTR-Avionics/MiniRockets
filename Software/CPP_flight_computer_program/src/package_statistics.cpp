#include "package_statistics.h"

/**
 * @brief  get exponential moving average
 * @param  new_value: new value to be added to the average
 * @param  old_avg: old average
 * @param  alpha_weight: weight of the new value
 * @note   strong smoothing: 0.1,   moderate smoothing: 0.5,   weak smoothing: 0.9
 */
float get_exponential_moving_average(float new_value, float old_avg, float alpha_weight)
	{
	return (new_value * alpha_weight) + old_avg * (1 - alpha_weight);
	}

/**
 * @brief    convert from radians to degrees
 * @param    r   the radians to convert
 * @return   the degrees
*/
float radians_to_degrees(float r)
	{
	return r * (float)(180 / M_PI);
	}

/**
 * @brief    convert from degrees to radians
 * @param    d   the degrees to convert
 * @return   the radians
*/
float degrees_to_radians(float d)
	{
	return d * (float)(M_PI / 180);
	}