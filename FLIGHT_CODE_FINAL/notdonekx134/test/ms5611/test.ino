#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initMS5611();
}

void loop() {
  // put your main code here, to run repeatedly:

  getMS5611_Values();
  Serial.print(rawTemp);
  Serial.print(",");
  Serial.print(realTemperature);
  Serial.print(",");
  Serial.print(rawPressure);
  Serial.print(",");
  Serial.print(realPressure);
  Serial.print(",");
  Serial.print(absoluteAltitude);
  Serial.print(",");
  Serial.print(relativeAltitude);


  


}
