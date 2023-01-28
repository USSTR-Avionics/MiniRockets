#ifndef SENSOR_FRAM_H
#define SENSOR_FRAM_H

// actual funcs
int init_fram();
int write_to_fram(int, int);
int read_from_fram(int);

#endif // SENSOR_FRAM_H