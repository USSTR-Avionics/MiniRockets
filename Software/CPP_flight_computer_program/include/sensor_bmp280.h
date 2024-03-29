#ifndef SENSOR_BMP280_H
#define SENSOR_BMP280_H

#define APOGEE_BUFFER_SIZE          5
#define APOGEE_READING_INTERVAL     50
#define APOGEE_DIFFERENCE_THRESHOLD 0.10f

extern float ground_base_pressure;
extern float ground_base_altitude;
extern float rocket_altitude;

int init_bmp280();
float get_bmp280_pressure();
float get_bmp280_altitude(float ground_base_pressure);
float get_bmp280_relative_altitude(float ground_base_pressure, float ground_base_altitude);

#endif // SENSOR_BMP280_H
