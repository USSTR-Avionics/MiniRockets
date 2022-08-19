/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_BMP280.h>


//SCL 19 SDA 18

Adafruit_BMP280 bmp; // I2C

void setup() {
  Serial.begin(9600);
    if (!bmp.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }


  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1); /* Standby time. */
}

void loop() {
    //Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.print(",");
    //Serial.println(" *C");

    //Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    //Serial.println(" Pa");
    Serial.print(",");

    //Serial.print(F("Approx altitude = "));
    Serial.println(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    //Serial.println(" m");
    delay(10);
    //Serial.println();
    //delay(2000);
}
