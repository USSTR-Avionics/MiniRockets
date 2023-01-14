float ms5611_temp   = FLO_DEF;
float ms5611_press  = FLO_DEF;
float ms5611_ground_base_pressure = FLO_DEF;

bool init_MS5611();
float get_ms5611_temp();
float get_ms5611_press();
double get_ms5611_altitude(double pressure, double seaLevelPressure);