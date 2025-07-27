/*
  MuxDemux.h - Library for using MuxDemux Module
  to increase Inputs and Outputs of a Microcontroller.
  Created by Aldrin G, September 16, 2023.
  https://github.com/aldrin7g
*/

#ifndef MUXDEMUX_H
#define MUXDEMUX_H

#include <Arduino.h>

// Function prototypes
void setupMuxDemux(int S0, int S1, int S2, int S3, int sig);
void digiWrite(int pinNumber, int value);
int MuxRead(int pinNumber);
void infoMuxDemux();
// Define global variables for pins
extern int S0;
extern int S1;
extern int S2;
extern int S3;
extern int sig;

#endif // MUXDEMUX_H
