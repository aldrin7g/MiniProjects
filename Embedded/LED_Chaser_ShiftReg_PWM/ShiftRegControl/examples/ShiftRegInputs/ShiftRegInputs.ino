#include "ShiftRegInputs.h"

void setup() {
  Serial.begin(9600);
  //SetupShiftRegInputs(<ClockEnable>, <ClockPin>, <LatchPin>, <DataPin>)
    setupShiftRegInputs(8,7,6,5); // Define your actual pin numbers
    info_IN();
}

void loop() {

//Example 1:
///*
  for(int pinNumber = 0; pinNumber<=7; pinNumber++){
    int pinValue = digiRead(pinNumber); // Call the user-defined function
    Serial.print("Pin ");
    Serial.print(pinNumber);
    Serial.print(" Value: ");
    Serial.println(pinValue);
  }
  Serial.println("\n");
  delay(1000);
//*/
//Example 2:
/*
  if (Serial.available() > 0) {
    int pinNumber = Serial.parseInt(); // Read the pin number from serial input
    int err_crt = Serial.read();
    int pinValue = digiRead(pinNumber); // Call the user-defined function
    Serial.print("Pin ");
    Serial.print(pinNumber);
    Serial.print(" Value: ");
    Serial.println(pinValue);
  }
  delay(1000); // Delay to avoid flooding the serial monitor
*/

//Example 3:
/*
  // Ask the user to select a pin and input value
  Serial.print("\nEnter a pin number (0-7): ");
  while (!Serial.available()) {
    // Wait for user input
  }
  int selectedPin = Serial.parseInt(); // Read the selected pin
  Serial.println(selectedPin);
  Serial.read(); // Clear the newline character ('\n')
  int pinValue = digiRead(selectedPin); // Call the user-defined function
  Serial.print("Value: ");
  Serial.println(pinValue);
  Serial.read(); // Clear the newline character ('\n')
*/
}
