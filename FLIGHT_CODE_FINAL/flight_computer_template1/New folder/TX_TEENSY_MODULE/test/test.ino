#include <SPI.h>
#include <RH_RF95.h>



void setup() {
  // put your setup code here, to run once:
    init_RFM95_TX();
}

void loop() {
  // put your main code here, to run repeatedly:
    sendPacket();
}
