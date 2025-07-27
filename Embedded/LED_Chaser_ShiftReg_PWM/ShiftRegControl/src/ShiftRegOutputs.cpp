#include "ShiftRegOutputs.h"

int dataPin;
int latchPin;
int clockPin;
int shiftRegisterState = 0; // Initialize the shift register state to all LOW

void setupShiftRegOutputs(int clock, int latch, int data) {
  dataPin  = data;
  latchPin = latch;
  clockPin = clock;

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  // Set all pins to LOW initially
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, shiftRegisterState);
  digitalWrite(latchPin, HIGH);
}

void digiWrite(int pinNumber, int value) {
  if ((pinNumber >= 0 && pinNumber <= 7) && (value == 1 || value == 0)) {
    // Check if the pin and value are valid
    if (value == 1) {
      // Set the selected pin to HIGH without affecting others
      shiftRegisterState |= (1 << pinNumber);
    } else {
      // Set the selected pin to LOW without affecting others
      shiftRegisterState &= ~(1 << pinNumber);
    }
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, shiftRegisterState);
    digitalWrite(latchPin, HIGH);
  } else {
    Serial.println("Invalid Input!");
  }
}

void info_OUT() {
  Serial.println("Pinout of the 74HC595 Shift Register IC (14 pins):");
  Serial.println("--------------------------------------------------");
  Serial.println("Pin 1:  Q1    - Output 1");
  Serial.println("Pin 2:  Q2    - Output 2");
  Serial.println("Pin 3:  Q3    - Output 3");
  Serial.println("Pin 4:  Q4    - Output 4");
  Serial.println("Pin 5:  Q5    - Output 5");
  Serial.println("Pin 6:  Q6    - Output 6");
  Serial.println("Pin 7:  Q7    - Output 7");
  Serial.println("Pin 8:  GND   - Ground");

  Serial.println("\nPin 9:  N/A   - Not Connected");
  Serial.println("Pin 10: MR    - Master Reset (Connect to VCC)");
  Serial.println("Pin 11: SH_CP - *Clock Input*");
  Serial.println("Pin 12: ST_CP - *Latch Input*");
  Serial.println("Pin 13: OE    - Output Enable (Connect to GND)");
  Serial.println("Pin 14: DS    - *Serial Data Output*");
  Serial.println("Pin 15: Q0    - Output 0");
  Serial.println("Pin 16: VCC   - Supply Voltage VCC");
  Serial.println("--------------------------------------------------");

  Serial.print("\nSetupShiftRegOutputs(<ClockPin>, <LatchPin>, <DataPin>)\n\n");
}