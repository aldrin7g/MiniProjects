//#include <SPI.h>
//#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//#define SS_PIN 10
//#define RST_PIN 9
//#define LED_PIN 3
int access = 0;
int access1 = 0;
String uid = "";
//MFRC522 mfrc522(SS_PIN, RST_PIN);

//candidate 1
const int BUTTON1 = 2;
const int LED1 = 3;
int BUTTONstate1 = 0;
int cand1 = 0;

//candidate 2
const int BUTTON2 = 4;
const int LED2 = 5;
int BUTTONstate2 = 0;
int cand2=0;

//candidate 3
const int BUTTON3 = 6;
const int LED3 = 7;
int BUTTONstate3 = 0;
int cand3 = 0;

//candidate 4
const int BUTTON4 = 8;
const int LED4 = 9;
int BUTTONstate4 = 0;
int cand4 = 0;

//NOTA
const int BUTTON5 = 10;
const int LED5 = 11;
int BUTTONstate5 = 0;
int nota = 0;

//buzzer
const int BUZZER=13;

//result
const int BUTTON6 = 12;
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
  {"ED9F9767", "Ligin Jitto E A", "URK22EC1035"},
  {"ODAA9767", "Dhayanithi M", "URK22EC1037"},
  {"3D618666", "Rithick I", "URK22EC1054"},
  {"ED1B9467", "Joshua JJ", "URK22EC1068"},
  {"8D6D6A66", "Sanjeevi J", "URK22EC1023"}  
};
Z
// define an array to keep track of which UIDs have already been scanned
bool scannedUids[8] = {false, false, false, false, false, false, false, false};

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  
  //SPI.begin();
  //mfrc522.PCD_Init();
  //pinMode(LED_PIN, OUTPUT);
  Serial.println("    RFID Reader ready!");
  //
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Scan Your ID!");
  //
  pinMode(BUTTON1, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON4, INPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUTTON5, INPUT);
  pinMode(LED5, OUTPUT);
  pinMode(BUTTON6, INPUT);
  
  pinMode(BUZZER,OUTPUT);
  
}

void loop() {
  // Look for new cards
  if (Serial.available() > 0) {
    // Show some details of the card
    Serial.println("\n*************************");
    //Serial.print("UID tag: ");
    String uid = "";
    uid = Serial.readString();
    uid.trim();
    uid.toUpperCase();
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
      if(uid != "DD65FF30"){
      Serial.println("    Authorized access    ");
      Serial.println("-------------------------");
      //digitalWrite(LED_PIN, HIGH);
      Serial.print("++++++User Details:++++++");
      Serial.print("\n-------------------------");
      Serial.print("\nName  : ");
      Serial.println(users[userIndex].name);
      Serial.print("Reg_No: ");
      Serial.println(users[userIndex].regno);
      Serial.println("*************************");
      //
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(users[userIndex].name);
      lcd.setCursor(0,1);
      lcd.print(users[userIndex].regno);
      //
      //delay(1000); // LED on for 1 second
      //digitalWrite(LED_PIN, LOW);
      }
        
        if ((uid == "DD65FF30")&&(access == 1)){
        Serial.println("++Admin Access Granted!++");
        Serial.print("*************************\n");
        //
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Admin Accessed!");
        lcd.setCursor(4,1);
        lcd.print("Results:");
        //
        }
        while (access == 1){
        BUTTONstate6 = digitalRead(BUTTON6);
        if ((BUTTONstate6  == HIGH)&&(uid == "DD65FF30"))
        {
          Serial.print("         RESULTS:\n");
          Serial.print("*************************\n");
          Serial.print("       Candidate1 ");
          Serial.println(cand1);
          Serial.print("       Candidate2 ");
          Serial.println(cand2); 
          Serial.print("       Candidate3 ");
          Serial.println(cand3); 
          Serial.print("       Candidate4 ");
          Serial.println(cand4); 
          Serial.print("          NOTA    ");
          Serial.println(nota); 
          Serial.print("*************************\n");
          Serial.print("          Total   ");
          Serial.println(cand1+cand2+cand3+cand4+nota);
          Serial.print("*************************\n");
          { 
            lcd.clear();
            lcd.setCursor(0,0);
            //lcd.print("Voting Success!");
            Serial.println("=========================");
            if (cand1 > cand2 && cand1 > cand3 && cand1 > cand4 && cand1 > nota) {
              Serial.println("     Candidate 1 won!");
              lcd.print("Candidate 1 won!");}
            else if (cand2 > cand1 && cand2 > cand3 && cand2 > cand4 && cand2 > nota) {
              Serial.println("     Candidate 2 won!");
              lcd.print("Candidate 2 won!");}
            else if (cand3 > cand1 && cand3 > cand2 && cand3 > cand4 && cand3 > nota) {
              Serial.println("     Candidate 3 won!");
              lcd.print("Candidate 3 won!");}
            else if (cand4 > cand1 && cand4 > cand2 && cand4 > cand3 && cand4 > nota) {
              Serial.println("     Candidate 4 won!");
              lcd.print("Candidate 4 won!");}
            else if (nota > cand1 && nota > cand2 && nota > cand3 && nota > cand4) {
              Serial.println("         NOTA Won!");
              lcd.print("    NOTA Won!");}
            else {
            Serial.println("          Draw!");
            lcd.print("     !Draw!");}
            Serial.println("=========================");
          }
          delay(1500);
          break;
        } 
        else if (uid != "DD65FF30"){
        BUTTONstate1 = digitalRead(BUTTON1);
        if (BUTTONstate1 == HIGH)
        {
          digitalWrite(LED1, HIGH);
          digitalWrite(BUZZER,HIGH);
          Serial.println("=========================");
          Serial.println("   Voted Successfully!   ");
          Serial.println("=========================");
          //
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Voting Success!");
          //
          cand1=cand1+1;
          delay(1500);
          break;
        } 
         BUTTONstate2 = digitalRead(BUTTON2);
        if (BUTTONstate2 == HIGH)
        {
          digitalWrite(LED2,HIGH);
          digitalWrite(BUZZER,HIGH);
          Serial.println("=========================");
          Serial.println("   Voted Successfully!   ");
          Serial.println("=========================");
          //
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Voting Success!");
          //
          cand2=cand2+1;
          delay(1500);
          break;
        }
         BUTTONstate3 = digitalRead(BUTTON3);
        if (BUTTONstate3 == HIGH)
        {
          digitalWrite(LED3,HIGH);
          digitalWrite(BUZZER,HIGH);
          Serial.println("=========================");
          Serial.println("   Voted Successfully!   ");
          Serial.println("=========================");
          //
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Voting Success!");
          //
          cand3=cand3+1;
          delay(1500);
          break;
        }
         BUTTONstate4 = digitalRead(BUTTON4);
        if (BUTTONstate4 == HIGH)
        {
          digitalWrite(LED4,HIGH);
          digitalWrite(BUZZER,HIGH);
          Serial.println("=========================");
          Serial.println("   Voted Successfully!   ");
          Serial.println("=========================");
          //
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Voting Success!");
          //
          cand4=cand4+1;
          delay(1500);
          break;
        }
         BUTTONstate5 = digitalRead(BUTTON5);
        if (BUTTONstate5 == HIGH)
        {
          digitalWrite(LED5,HIGH);
          digitalWrite(BUZZER,HIGH);
          Serial.println("=========================");
          Serial.println("   Voted Successfully!   ");
          Serial.println("=========================");
          //
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Voting Success!");
          //
          nota=nota+1;
          delay(1500);
          break;
        }
          }
            }
          }
          else if ((uid == "DD65FF30") && (access1 == 0)){
            Serial.println("++Admin Access Granted!++");
            Serial.print("*************************\n");
            //
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("Admin Accessed!");
            //
            cand1 = cand2 = cand3 = cand4 = nota = 0;
            access1 = 1;
          }
          else {
            // access denied
            Serial.println("++++++Access denied++++++");
            Serial.print("*************************\n");
            //
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("Access Denied!");
            //
          }
          digitalWrite(LED1,LOW);
          digitalWrite(LED2,LOW);
          digitalWrite(LED3,LOW);
          digitalWrite(LED4,LOW);
          digitalWrite(LED5,LOW);
          digitalWrite(BUZZER,LOW);
          }
}
