#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define RST_PIN 0 // Define the reset pin for the RC522 RFID module
#define SS_PIN 15 // Define the SS (Slave Select) pin for the RC522 RFID module

MFRC522 mfrc522(SS_PIN, RST_PIN);//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);

//
int access = 0;
int access1 = 0;
MFRC522 mfrc522(SS_PIN, RST_PIN);

//candidate 1
const int BUTTON1 = 2;
int BUTTONstate1 = 0;
int cand1=0;

//candidate 2
const int BUTTON2 = 4;
int BUTTONstate2 = 0;
int cand2=0;

//result
const int BUTTON6 = 7;
int BUTTONstate6 = 0;

// define the user details struct
struct User {
  String uid;
  String name;
  String regno;
};

// create an array of User structs to store user details for each UID
User users[] = {
  {"DD65FF30", "Admin", ""},
  {"2DA77566", "Allen Benshiel J", "URK22EC1027"},
  {"2D889A67", "Aldrin G", "URK22EC1019"},
  {"7535ABAC", "Ligin Jitto E A", "URK22EC1035"}
};

// define an array to keep track of which UIDs have already been scanned
bool scannedUids[4] = {false, false, false, false};
//

void setup() {
  //Serial.begin(9600);
  s.begin(9600); // Start the serial communication
  SPI.begin(); // Start the SPI communication
  mfrc522.PCD_Init(); // Initialize the MFRC522 RFID module

  //
  pinMode(BUTTON1, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(BUTTON6, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(LED2, OUTPUT);
  //
  
}

void loop() {
 // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Show some details of the card
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      //Serial.print(mfrc522.uid.uidByte[i], HEX);
      uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      uid.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    // Print the UID of the card
    uid.toUpperCase();
    s.println(uid);
    
    int userIndex = -1;
    for (int i = 0; i < 4; i++) {
      if (uid == users[i].uid && !scannedUids[i] && access1 == 1) {
        userIndex = i;
        if(uid != "DD65FF30")
        scannedUids[i] = true;
        break;
      }
    }
    
    if ((userIndex != -1) && (access1 == 1)) {
      // access granted
      access = 1;
      
        while (access == 1){
        BUTTONstate6 = digitalRead(BUTTON6);
        if ((BUTTONstate6  == HIGH)&&(uid == "DD65FF30"))
            {
            //
            break;
            }
        else if (uid != "DD65FF30"){
        BUTTONstate1 = digitalRead(BUTTON1);
        if (BUTTONstate1 == HIGH)
        {
         cand1=cand1+1;
         break;
        } 
        BUTTONstate2 = digitalRead(BUTTON2);
        if (BUTTONstate2 == HIGH)
        {
          cand2=cand2+1;
          break;
        }}}}
          else if ((uid == "DD65FF30") && (access1 == 0)){
            cand1 = cand2 = 0;
            access1 = 1;
          }
        }
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
      }
