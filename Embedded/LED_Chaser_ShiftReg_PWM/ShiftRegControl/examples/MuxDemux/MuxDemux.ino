#include "MuxDemux.h"

void setup() {
  //setupMuxDemux(S0, S1, S2, S3, sig);
    setupMuxDemux(2, 3, 4, 5, 6);
}

void loop() {
//Example 1:
  /*
  MuxWrite(1, HIGH);
  delay(1000);

  // Use digiRead to read the state of the pin
  int value = MuxRead(7);
  Serial.println(value);
  delay(1000);
  */

//Example 2:
  for (int i = 0; i <= 15; i++){
    Serial.println(MuxRead(i));
  }
}
