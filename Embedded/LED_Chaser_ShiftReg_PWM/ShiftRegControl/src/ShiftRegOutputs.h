/*
  ShiftRegOutputs.h - Library for using ShiftRegister 74HC595
  to increase Inputs and Outputs of a Microcontroller.
  Created by Aldrin G, September 16, 2023.
  https://github.com/aldrin7g
*/

#ifndef ShiftRegOutputs_h
#define ShiftRegOutputs_h

#include <Arduino.h>

// Function prototypes
void setupShiftRegOutputs(int clockPin, int latchPin, int dataPin);
void digiWrite(int pinNumber, int value);
void info_OUT();
#endif // ShiftRegOutputs_h
