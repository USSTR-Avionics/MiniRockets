

/*
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
