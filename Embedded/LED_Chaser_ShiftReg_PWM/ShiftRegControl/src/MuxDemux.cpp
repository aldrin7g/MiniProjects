#include "MuxDemux.h"

// Define global variables for pins
int S0;
int S1;
int S2;
int S3;
int sig;

// Function definitions
void setupMuxDemux(int S0, int S1, int S2, int S3, int sig) {
  if ((S0 >= 0 && S0 <= 15) && (S1 >= 0 && S1 <= 15) && (S2 >= 0 && S2 <= 15) &&
      (S3 >= 0 && S3 <= 15) && (sig >= 0 && sig <= 15)) {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    Serial.begin(9600);
    
    // Assign values to global variables
    ::S0 = S0;
    ::S1 = S1;
    ::S2 = S2;
    ::S3 = S3;
    ::sig = sig;
  } 
  else {
    Serial.println("Invalid setup parameters. Use the following Syntax:");
    //Serial.println("setupMuxDemux(S0, S1, S2, S3, sig);");
  }
}

void MuxWrite(int pinNumber, int value) {
  pinMode(::sig, OUTPUT);
  // Check if pinNumber is within the valid range (0-15)
  if (pinNumber >= 0 && pinNumber <= 15) {
    // Convert pinNumber to a 4-bit binary representation
    digitalWrite(::S0, (pinNumber & B0001) ? HIGH : LOW);
    digitalWrite(::S1, (pinNumber & B0010) ? HIGH : LOW);
    digitalWrite(::S2, (pinNumber & B0100) ? HIGH : LOW);
    digitalWrite(::S3, (pinNumber & B1000) ? HIGH : LOW);

    // Set the specified pin to the given value (HIGH or LOW)
    digitalWrite(::sig, value);
  } else {
    Serial.println("Invalid pin number. Please use a value between 0 and 15.");
    return -1; // Return an error value
  }
}

int MuxRead(int pinNumber) {
  pinMode(::sig, INPUT);
  // Check if pinNumber is within the valid range (0-15)
  if (pinNumber >= 0 && pinNumber <= 15) {
    // Convert pinNumber to a 4-bit binary representation
    digitalWrite(::S0, (pinNumber & B0001) ? HIGH : LOW);
    digitalWrite(::S1, (pinNumber & B0010) ? HIGH : LOW);
    digitalWrite(::S2, (pinNumber & B0100) ? HIGH : LOW);
    digitalWrite(::S3, (pinNumber & B1000) ? HIGH : LOW);

    // Read the state of the specified pin and return it
    return digitalRead(::sig);
  } else {
    Serial.println("Invalid pin number. Please use a value between 0 and 15.");
    return -1; // Return an error value
  }
}

void infoMuxDemux(){
  Serial.println("\nsetupMuxDemux(S0, S1, S2, S3, sig);\n");
}