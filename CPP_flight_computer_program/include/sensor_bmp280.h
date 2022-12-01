float ground_base_pressure = FLO_DEF;
float ground_base_altitude = FLO_DEF;
float rocket_altitude   = 0.0;

int init_bmp280();
float get_bmp280_pressure();
float get_bmp280_altitude(float ground_base_pressure);
float get_bmp280_relative_altitude(float ground_base_pressure, float ground_base_altitude);