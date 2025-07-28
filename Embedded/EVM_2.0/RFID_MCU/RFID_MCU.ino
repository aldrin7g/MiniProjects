#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <SoftwareSerial.h>
//#define TXp2 17

#define RST_PIN 4 // Define the reset pin for the RC522 RFID module
#define SS_PIN  5 // Define the SS (Slave Select) pin for the RC522 RFID module

MFRC522 mfrc522(SS_PIN, RST_PIN);//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
//SoftwareSerial s(3,1);

void setup() {
  Serial.begin(9600);
  //s.begin(9600); // Start the serial communication
  SPI.begin(); // Start the SPI communication
  mfrc522.PCD_Init(); // Initialize the MFRC522 RFID module
  //Serial.println("Ready to read RFID tags...");
}

void loop() {
 // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Show some details of the card
    // Serial.println("\n*************************");
    //Serial.print("UID tag: ");
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      //Serial.print(mfrc522.uid.uidByte[i], HEX);
      uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      uid.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    // Print the UID of the card
    //Serial.print("UID tag : ");
    uid.toUpperCase();
    Serial.println(uid);
    // Stop reading RFID cards          
    mfrc522.PICC_HaltA();
    // Stop the encryption on the card
    mfrc522.PCD_StopCrypto1();
  }
}
