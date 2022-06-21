


bool initKX134() {
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

void getKX134_Accel() {
  kx134_accel = kxAccel.getAccelData(); 
  kx134_accel_x = kx134_accel.xData*9.81108;
  kx134_accel_y = kx134_accel.yData*9.81108;
  kx134_accel_z = kx134_accel.zData*9.81108;
}

/*void setup() {


  
}*/

/*void loop() {

  myData = kxAccel.getAccelData(); 


  delay(20); // Delay should be 1/ODR (Output Data Rate), default is 50Hz

}*/
