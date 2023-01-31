#ifndef SENSOR_FRAM_H
#define SENSOR_FRAM_H

// actual funcs
int init_fram();
int write_to_fram(int, int); // this should not be exposed
int read_from_fram(int);     // this should not be exposed

#endif // SENSOR_FRAM_H
