#include "ShiftRegInputs.h"

int ShiftLoad;
int ClockEnable;
int Clock;
int Data;

void setupShiftRegInputs(int clockEnablePin, int clockPin, int shiftLoadPin, int dataPin) {
  ShiftLoad = shiftLoadPin;
  ClockEnable = clockEnablePin;
  Clock = clockPin;
  Data = dataPin;

  pinMode(ShiftLoad, OUTPUT);
  pinMode(ClockEnable, OUTPUT);
  pinMode(Clock, OUTPUT);
  pinMode(Data, INPUT);
}

int digiRead(int pinNumber) {
  // Ensure pinNumber is in the range 0-7
  if (pinNumber >= 0 && pinNumber <= 7) {
    digitalWrite(ShiftLoad, LOW);
    delayMicroseconds(5);
    digitalWrite(ShiftLoad, HIGH);
    delayMicroseconds(5);

    digitalWrite(Clock, HIGH);
    digitalWrite(ClockEnable, LOW);
    byte incoming = shiftIn(Data, Clock, LSBFIRST);
    digitalWrite(ClockEnable, HIGH);
    
    // Extract the value of the specified pin from the incoming byte
    if(((incoming >> pinNumber) & 0x01) == 1)
     return 0;
    else if(((incoming >> pinNumber) & 0x01) == 0)
     return 1;
  } else {
    Serial.println("Invalid pin number. Please enter a number between 0 and 7!");
    return -1; // Return an error value
  }
}

void info_IN() {
  delay(1000);
  Serial.println("Pinout of the 74HC595 Shift Register IC (14 pins):");
  Serial.println("--------------------------------------------------");
  Serial.println("Pin 1:  ST_CP - *Latch Input*");
  Serial.println("Pin 2:  SH_CP - *Clock Input*");
  Serial.println("Pin 3:  Q3    - Input 3");
  Serial.println("Pin 4:  Q2    - Input 2");
  Serial.println("Pin 5:  Q1    - Input 1");
  Serial.println("Pin 6:  Q0    - Input 0");
  Serial.println("Pin 7:  N/A   - Not Connected");
  Serial.println("Pin 8:  GND   - Ground");

  Serial.println("\nPin 9:  DS  - *Serial Data Input*");
  Serial.println("Pin 10: N/A   - Not Connected");
  Serial.println("Pin 11: Q7    - Input 7");
  Serial.println("Pin 12: Q6    - Input 6");
  Serial.println("Pin 13: Q5    - Input 5");
  Serial.println("Pin 14: Q4    - Input 4");
  Serial.println("Pin 15: CE    - *Clock Enable*");
  Serial.println("Pin 16: VCC   - Supply Voltage VCC");
  Serial.println("--------------------------------------------------");

  Serial.print("\nSetupShiftRegInputs(<ClockEnable>, <ClockPin>, <LatchPin>, <DataPin>)\n\n");
}