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

const int chipSelect = BUILTIN_SDCARD;
String dataString = "";

void initMicroSD() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  if (microSdActivate == 1)
  {
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
}

void serialPrint()
{
//  
  dataString.concat(x);
  dataString.concat(",");
//  dataString.concat(bno055_accel_x);
//  dataString.concat(",");
//  dataString.concat(bno055_accel_y);
//  dataString.concat(",");
//  dataString.concat(bno055_accel_z);
//  dataString.concat(",");
//  dataString.concat(bno055_linear_x);
//  dataString.concat(",");
//  dataString.concat(bno055_linear_y);
//  dataString.concat(",");
//  dataString.concat(bno055_linear_z);
//  dataString.concat(",");
//  dataString.concat(bno055_mag_x);
//  dataString.concat(",");
//  dataString.concat(bno055_mag_y);
//  dataString.concat(",");
//  dataString.concat(bno055_mag_z);
//  dataString.concat(",");
//  dataString.concat(bno055_gravity_x);
//  dataString.concat(",");
//  dataString.concat(bno055_gravity_y);
//  dataString.concat(",");
//  dataString.concat(bno055_gravity_z);
//  dataString.concat(",");
//  dataString.concat(bno055_ang_vel_x);
//  dataString.concat(",");
//  dataString.concat(bno055_ang_vel_y);
//  dataString.concat(",");
//  dataString.concat(bno055_ang_vel_z);
//  dataString.concat(",");
//  dataString.concat(bno055_euler_x);
//  dataString.concat(",");
//  dataString.concat(bno055_euler_y);
//  dataString.concat(",");
//  dataString.concat(bno055_euler_z);
//  dataString.concat(",");
//  dataString.concat(bno055_quat_w);
//  dataString.concat(",");
//  dataString.concat(bno055_quat_y);
//  dataString.concat(",");
//  dataString.concat(bno055_quat_x);
//  dataString.concat(",");
//  dataString.concat(bno055_quat_z);
//  dataString.concat(",");
//  dataString.concat(bno055_temp);
//  dataString.concat(",");
//  dataString.concat(rawTemp);
//  dataString.concat(",");
//  dataString.concat(rawPressure);
//  dataString.concat(",");
//  dataString.concat(realTemperature);
//  dataString.concat(",");
//  dataString.concat(realPressure);
//  dataString.concat(",");
//  dataString.concat(absoluteAltitude);
//  dataString.concat(",");
  if (KX134Activate == 1)
  {
    dataString.concat(kx134_accel_x);
    dataString.concat(",");
    dataString.concat(kx134_accel_y);
    dataString.concat(",");
    dataString.concat(kx134_accel_z);
    dataString.concat(",");
  }

  dataString.concat(decentCheck);
  dataString.concat(",");
  dataString.concat(currentState);
  dataString.concat(",");
  dataString.concat(relativeAltitude);
  Serial.println(dataString);
  dataString = "";
}

void writeToMicroSD() {
  
  if (microSdActivate == 1)
  {
      // make a string for assembling the data to log:
Serial.print("IM WRITING");
  // open the file.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.print("WROTE TO SD:");
    //Serial.println(dataString);
  } else {
    // if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
  }
  }

}
