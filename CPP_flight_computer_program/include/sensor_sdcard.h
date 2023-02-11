#ifndef SENSOR_SDCARD_H
#define SENSOR_SDCARD_H

const char* const datalog_fmt_header = "time, state, kx134_x, kx134_y, kx134_z, bmp280_alt";

#define DATALOG  0
#define EVENTLOG 1

int init_SD();
auto write_to_sd_card(int event_type, const char* str) -> int;

#endif // SENSOR_SDCARD_H
