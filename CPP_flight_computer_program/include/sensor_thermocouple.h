#ifndef SENSOR_THERMOCOUPLE_H
#define SENSOR_THERMOCOUPLE_H

#define MAXDO  3
#define MAXCS  4
#define MAXCLK 5

void init_thermocouple();
float get_external_temperature();
float get_internal_temperature();

#endif