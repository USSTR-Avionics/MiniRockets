#include "Adafruit_BMP280.h"


Adafruit_BMP280 bmp; // use I2C interface
// Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();


int init_bmp280()
    {
    if (!bmp.begin())
        {
        /* Default settings from datasheet. */
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
            Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
            Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
            Adafruit_BMP280::FILTER_X16,      /* Filtering. */
            Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
        }

    return EXIT_SUCCESS;
    }

float get_bmp280_altitude(float ground_base_pressure)
    {
    return bmp.readAltitude(ground_base_pressure);
    }

float get_bmp280_pressure()
    {
    return bmp.readPressure();
    }