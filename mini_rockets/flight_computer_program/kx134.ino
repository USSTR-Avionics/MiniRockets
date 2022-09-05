/*
 * KX134 Module
 * 
 * Product Link: https://www.sparkfun.com/products/17589
 * 
 * Inputs: GND, 3V3, SDA, SCL, ADR, CS
 * 
 * Outputs:
 * - rawTemp
 * - rawPressure 
 * - realTemperature
 * - realPressure
 * - absoluteAltitude
 * - realAltitude
 * 
 * Set Up Instructions: 
 * 1. Connect 3V3 to 3V3 pin and GND to GND
 * 2. Connect SCL to SCL (PIN 19 ON TEENSY) and SDA to SDA (PIN 18 ON TEENSY)
 *
 * Functions:
 * bool initKX134(): initializes the sensor (0=error, 1=success)
 * getKX134_Accel(): updates kx134_accel, kx134_accel_x, kx134_accel_y, kx134_accel_z global variables
 */


bool initKX134() {
  if (KX134Activate == 1)
  {
    if( !kxAccel.begin() ){
    Serial.println("Could not communicate with the the KX13X. Freezing.");
    return 0;
  }
  else
    Serial.println("Ready.");
  if( !kxAccel.initialize(DEFAULT_SETTINGS)){ // Loading default settings.
    Serial.println("Could not initialize the chip.");
    return 0;
  }
  else
    Serial.println("KX134 Initialized...");
  // kxAccel.setRange(KX132_RANGE16G);
  // kxAccel.setRange(KX134_RANGE32G); // For a larger range uncomment
  return 1;
  }

}

void getKX134_Accel() {
  if (KX134Activate == 1)
  {
  kx134_accel = kxAccel.getAccelData(); 
  kx134_accel_x = kx134_accel.xData*9.81108;
  kx134_accel_y = kx134_accel.yData*9.81108;
  kx134_accel_z = kx134_accel.zData*9.81108;
  }

}
