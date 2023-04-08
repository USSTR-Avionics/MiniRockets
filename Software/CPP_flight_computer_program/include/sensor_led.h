#ifndef SENSOR_LED_H
#define SENSOR_LED_H

int init_LED();

void set_led_red();
void set_led_green();
void set_led_blue();
void reset_led();

void flash_internal_led(bool);

#endif // SENSOR_LED_H