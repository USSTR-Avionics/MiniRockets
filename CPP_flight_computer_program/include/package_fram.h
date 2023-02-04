#ifndef PACKAGE_FRAM_H
#define PACKAGE_FRAM_H

int write_temperature_to_fram(float);
int write_acceleration_to_fram(float);
float read_temperature_from_fram(int where);
float read_acceleration_from_fram(int where);

#endif // PACKAGE_FRAM_H