#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Wire.h>

int corrector = 0;
int trigger = 15;
int echo = 13;
float floodHeight, distance, height;
long duration;
const float floodThreshold = 24.5;

//----------- Enter you Wi-Fi Details---------//
char ssid[] = "Virus#404"; //SSID
char pass[] = "password1"; // Password
//-------------------------------------------//

WiFiClient  client;
unsigned long myChannelField = 2125130; // Channel ID
const int ChannelField = 1; // Which channel to write data
const char * myWriteAPIKey = "8G4IL36HH1UYH6SI"; // Your write API Key


void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Wire.begin();
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
}

void loop(){
 
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  duration = pulseIn(echo,HIGH);
  distance = duration*(0.0176);
  delay(100);
  
  int sensorHeight = 0;
  floodHeight = (floodThreshold-(sensorHeight + distance));
  corrector = floodHeight;
  if(corrector<0)
  corrector = 0;
  Serial.print("Distance: ");
  Serial.print(corrector);
  Serial.println(" Cm");
  ThingSpeak.writeField(myChannelField, ChannelField, corrector, myWriteAPIKey);
}
 
