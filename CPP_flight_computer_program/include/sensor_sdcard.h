#ifndef SENSOR_SDCARD_H
#define SENSOR_SDCARD_H

const char* const datalog_fmt_header = "time, state, kx134_x, kx134_y, kx134_z, bmp280_alt";

#define DATALOG  0
#define EVENTLOG 1

int init_SD();
int write_to_sd_card(int f, const char* str);

#endif // SENSOR_SDCARD_H