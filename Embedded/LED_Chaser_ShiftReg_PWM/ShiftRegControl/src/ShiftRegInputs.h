/*
  ShiftRegInputs.h - Library for using ShiftRegister 74HC165
  to increase Inputs and Outputs of a Microcontroller.
  Created by Aldrin G, September 16, 2023.
  https://github.com/aldrin7g
*/

#ifndef ShiftRegInputs_h
#define ShiftRegInputs_h

#include "Arduino.h"

// Function prototypes
void setupShiftRegInputs(int clockEnablePin, int clockPin, int shiftLoadPin, int dataPin);
int digiRead(int pinNumber);
void info_IN();
#endif // ShiftRegInputs_h
