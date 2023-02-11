#ifndef SENSOR_LED_H
#define SENSOR_LED_H

int init_LED();
void setLedGreen();
void setLedRed();
void setLedBlue();
void flashSOS();
void flashInternalLed(bool);

#endif // SENSOR_LED_H