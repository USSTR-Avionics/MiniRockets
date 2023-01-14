/*
  MicroSD Module
 
 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 THIS EXAMPLE READS FROM A9 PIN ON TEENSY RIGHT NOW
   
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
 * - i.e. Change chipSelect = BUILTIN_SDCARD for Teensy 4.1
 * 
 * Functions:
 * initMicroSD():initializes the MicoSD module
 * writeToMicroSD(): writes the value at A9 to the microSD card
 * 
 */

#include <SD.h>
#include <SPI.h>

float x =1;
float y = 76.22;
float z = 1.23456;

const int chipSelect = BUILTIN_SDCARD;



void setup()
{
  initMicroSD();
  
}

void initMicroSD() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1) {
      // No SD card, so don't do anything more - stay stuck here
    }
  }
  Serial.println("card initialized.");
}

void writeToMicroSD() {
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  /*for (int analogPin = 7; analogPin < 10; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }*/
  
  dataString = "";
  dataString.concat(x);
  dataString.concat(y);
  dataString.concat(z);
  x=x+1;

  // open the file.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  } else {
    // if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
  }
  delay(100); // run at a reasonable not-too-fast speed
}

void loop()
{
  writeToMicroSD();
}
