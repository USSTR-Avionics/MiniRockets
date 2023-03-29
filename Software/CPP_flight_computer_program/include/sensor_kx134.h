#ifndef SENSOR_KX134_H
#define SENSOR_KX134_H

extern float kx134_accel_x;
extern float kx134_accel_y;
extern float kx134_accel_z;

bool init_kx134();
float get_kx134_accel_x(); // returns raw accel
float get_kx134_accel_y(); // returns raw accel
float get_kx134_accel_z(); // returns raw accel

#endif // SENSOR_KX134_H
