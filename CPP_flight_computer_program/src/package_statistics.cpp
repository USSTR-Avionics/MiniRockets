
float get_exponential_moving_average(float new_value, float old_avg, float alpha_weight)
    {
    return (new_value * alpha_weight) + old_avg * (1 - alpha_weight);
    }
