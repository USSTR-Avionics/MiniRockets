/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
  uint8_t store_array_counter=0, tmp_uint8;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 38400 bits per second:
  Serial.begin(38400); // 38400 teensy, 9600 arduino uno
 
}

// the loop routine runs over and over again forever:
void loop() {
 
  // read the input on analog pin 0:
  int sensorValue = analogRead(A13);  // A1 For Arduino, A13 For Teensy
  tmp_uint8 = (uint8_t)((analogRead(A13) >> 4)&0xFF); // NTC internal A1 For Arduino, A13 For Teensy
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = ((sensorValue * (3.3 / 1023.0))-0.5)*100*-1;
  // print out the value you read:
  delayMicroseconds(1000000);
  Serial.println(voltage);
  delay(100);
}
