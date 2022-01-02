/*
 * GPS_NEO6M Module
 * 
 * Inputs: Vcc,GND,Tx,Rx
 * 
 * Outputs:
 * -latitude:
 * -longitude:
 * 
 * Set Up Instructions:
 * 1. Connect Vcc pin to 3.3 V (WARNING: DO NOT EVER CONNECT TO 5V, 
 * THIS WILL FRY THE MODULE), and GND pin to a ground pin of the Arduino
 * 2. Connect Tx and Rx pins to the Tx and Rx pins of the Arduino
 * 3. Open a new Arduino program
 * 4. Disconnect the Tx and Rx pins of the module
 * 5. Upload an empty program onto the module
 * 6. Connect the Tx and Rx pins to the module
 * 7. Open the Serial monitor and set the baud rate to 9600
 * 8. Get a GPS fix, fix occurs when the data fills up with numbers
 * 9. Once $GPGLL is filled with values, copy and paste this in http://freenmea.net/decoder to obtain GPS location
 * 10. Example: $GPGLL,5206.91616,N,10639.19414,W,184918.00,A,A*7A 

TODO:
- Figure out what other data can be retrieved from this sensor and add them to the outputs
- Make a function that returns the outputs

 */


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
