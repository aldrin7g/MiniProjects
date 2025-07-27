#include "ShiftRegOutputs.h"

int selectedPin;
int inputValue;

void setup() {
  Serial.begin(9600); // Initialize Serial communication
  //SetupShiftRegOutputs(<ClockPin>, <LatchPin>, <DataPin>)
    setupShiftRegOutputs(12,11,10); // Define your actual pin numbers
    info_OUT();
}

void loop() {

//Example 1:
///*
for(int pinNumber = 0; pinNumber<=7; pinNumber++){
    digiWrite(pinNumber, HIGH);
    delay(1000);
    digiWrite(pinNumber, LOW);
  }
//*/

//Example 2:
/*
  // Ask the user to select a pin and input value
  // (You can modify this part of the code to take inputs from the user.)
  Serial.print("\nEnter a pin number (0-7): ");
  while (!Serial.available()) {
    // Wait for user input
  }
  selectedPin = Serial.parseInt(); // Read the selected pin
  Serial.println(selectedPin);
  Serial.read(); // Clear the newline character ('\n')

  Serial.print("Enter a Value (0 or 1): ");
  while (!Serial.available()) {
    // Wait for user input
  }
  inputValue = Serial.parseInt(); // Read the input value
  Serial.println(inputValue);
  Serial.read(); // Clear the newline character ('\n')

  digiWrite(selectedPin, inputValue);
*/
}
