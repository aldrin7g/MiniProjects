#include "ShiftRegInputs.h"
#include "ShiftRegOutputs.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Initialize Serial communication
  //SetupShiftRegOutputs(<ClockPin>, <LatchPin>, <DataPin>)
    setupShiftRegOutputs(12,11,10); // Define your actual pin numbers
  //SetupShiftRegInputs(<ClockEnable>, <ClockPin>, <LatchPin>, <DataPin>)
    setupShiftRegInputs(8,7,6,5); // Define your actual pin numbers
}

void loop() {

//Example 1:
int Button = 7;
int Led1 = 1;
int Led2 = 2;

if(digiRead(Button) == HIGH){
  digiWrite(Led1, HIGH);
  digiWrite(Led2,LOW);
}

else{
  digiWrite(Led1, LOW);
  digiWrite(Led2,HIGH);
  }
}
