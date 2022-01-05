/* 
 *   TemperatureNTC Module
 * - Steinhart Hart Equation is used to calculate the temperature
 *   by measuring the voltage in the voltage divider of R1 and R2
 *   
 *   tempPin: Voltage between R1 and R2
 *   R1: The fixed resistance value
 *   R2: thermistor resistance
 *   lnR2: ln(R2)
 *   tempK: Temperature (K)
 *   tempC: Temperature (C)
 *   c1,c2,c3: Constants used in the steinhart hart equation to calculate temperature
 *  
 *  Steps:
 *  1. Connect 5V pin on microcontroller to the NTC thermistor
 *  2. Connect the NTC thermistor to a 10 kohm resistor
 *  3. Connect the resistor to ground
 *  4. Connect an analog pin (here we use A0) to measure the voltage between the thermistor and resistor
 *  NOTE C1, C2, C3 Constants will need to be modified for different thermistors
 *  
 *  TODO: EVERYTHING BELOW THIS LINE
 *  Functions:
 *  temperatureNTC(pin) -> temperature: returns the temperature given by the NTC thermistor
 *  
 *  Steps to Determine C1, C2, C3:
 *  1. Connect this program to the NTC thermistor in the arduino sensor pack
 *  2. Confirm correct operation of the program (i.e. the correct temperature is displayed)
 *  3. Calculate C1, C2, and C3 and confirm it matches the code given below i.e.
 *  3.1 Go to https://www.northstarsensors.com/calculating-temperature-from-resistance
 *  3.2 Read the information given on the website first THEN click on Steinhart-Hart Coefficients Calculator - Excel Version
 *  3.3 Measure the resistance of the NTC thermistor, under zero power conditions, at four temperature points, where T1 is the lowest temperature of the range, T2 and T3 are the middle temperatures, and T4 is the highest temperature of the range
 *  3.4 Plug the measurements into the spreadsheet and confirm they match the following (NOTE C1 = A, C2 = B, C3 = D):
 *  - C1 = 0.001129148
 *  - C2 = 0.000234125
 *  - C3 = 0.0000000876741
 *  4. Repeat step 3 for the thermistor used
 *  5. The ln^2 term is not included in the steinhart equation in this code
 *  - double tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * lnR2 * lnR2 )) * lnR2 ); 
 *  - Why is this the case? Hint check https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation and look in the derivation and alternatives section
 *  - Based on this information,do you think the ln^2 term should be included?. If yes, then add it to the equation
 *  
 */


int tempPin = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("TEST");

}
void loop()
{
  int tempReading = analogRead(tempPin);
  // Find ln of R2 by ln of voltage divider equation R2 = R1* (Vin/Vout-1)
  double lnR2 = log(10000.0 * ((1024.0 / tempReading - 1)));
  // Convert resistance to temperature using steinhart-hart equation
  double tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * lnR2 * lnR2 )) * lnR2 );       
  // Convert Kelvin to Celcius
  float tempC = tempK - 273.15;            
  // Display Temperature in C
  Serial.println(tempC);
  delay(500);
}
