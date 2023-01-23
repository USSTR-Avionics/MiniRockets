#ifndef SENSOR_FRAM_H
#define SENSOR_FRAM_H

// actual funcs
int init_fram();
int write_temperature_to_fram(float);
int write_acceleration_to_fram(float);

#endif // MEMORY_FRAM_H
