float ground_base_pressure = FLO_DEF;
float rocket_altitude   = 0.0;

int init_bmp280();
float get_bmp280_pressure();
float get_bmp280_altitude(float);