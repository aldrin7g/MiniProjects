#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

const char* ssid = "Virus#404";       // Replace with your Wi-Fi SSID
const char* password = "password2";   // Replace with your Wi-Fi password

FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

Servo myServo;

#define SS_PIN D8
#define RST_PIN D4
#define LOCK D3
#define BUTTON D0

int lockState = 0;
bool BotFlag = false;
int prevLockState = -1; // Initialize with an invalid state

// Create RFID object
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Create LCD object (I2C address 0x27, 16x2 LCD)
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
    Serial.begin(9600);
    pinMode(BUTTON, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Connecting WiFi!");

    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" WiFi Connected ");
    delay(500);

    //pinMode(LOCK, OUTPUT);
    myServo.attach(LOCK);
    myServo.write(0);
    
    // Set up Firebase
    firebaseConfig.host = "iotaldrin-default-rtdb.firebaseio.com";
    firebaseConfig.signer.tokens.legacy_token = "UD5vdGwG5xTpTouDnBOJ4wWZxIu3hsFgm8psOdvC"; // Replace with your Firebase credentials
    
    Firebase.begin(&firebaseConfig, &firebaseAuth);
    Firebase.reconnectWiFi(true);

    Firebase.setInt(firebaseData, "/Photo", 0);
    Firebase.setInt(firebaseData, "/Lock", 0);
    Firebase.setInt(firebaseData, "/LockBot", 0);
    
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   SmartLock!   ");
}

void loop() {

    if(prevLockState == -1){
        if (Firebase.getInt(firebaseData, "/Lock") && (firebaseData.dataType() == "int")) {
          lockState = firebaseData.intData();
          Firebase.setInt(firebaseData, "/LockBot", lockState);
          digitalWrite(LOCK, lockState);
          prevLockState = lockState; // Update previous value
        }
    }

    // Check Firebase for the lock status
    else if((Firebase.getInt(firebaseData, "/LockBot") && (firebaseData.dataType() == "int")) && !BotFlag) {
        int newLockState = firebaseData.intData();
        // Update lockState only if it has changed
        if (newLockState != prevLockState) {
            lockState = !newLockState;
            BotFlag = true;
            prevLockState = newLockState; // Update previous value
        }
    }

    if (digitalRead(BUTTON) == HIGH){
       Firebase.setInt(firebaseData, "/Photo", 1);
       delay(1000);
    }

    // Look for new RFID card
    if (BotFlag || (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())) {
        Serial.print("UID tag: ");
        String content = "";
        byte letter;
        
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            Serial.print(mfrc522.uid.uidByte[i], HEX);
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            content.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        Serial.println();
        Serial.print("Message: ");
        
        content.toUpperCase();

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("UID: ");
        lcd.setCursor(0, 1);
        lcd.print(content.substring(1));

        if  ((BotFlag) || 
            ((content.substring(1) == "DD 65 FF 30") || 
            (content.substring(1) == "75 35 AB AC") || 
            (content.substring(1) == "2D 88 9A 67") || 
            (content.substring(1) == "2D A7 75 66")))  {
              
            
            Serial.println("Authorized access");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Access Granted ");

          if (lockState) {
              Firebase.setInt(firebaseData, "/Lock", 0);
              Firebase.setInt(firebaseData, "/LockBot", 0);
              //digitalWrite(LOCK, LOW);
              myServo.write(0); // Unlocked position
              lockState = 0;
              lcd.setCursor(0, 1);
              lcd.print("  Lock Opened!  ");
              if (!BotFlag)
                Firebase.setInt(firebaseData, "/Photo", 1);
              delay(2000);
            }
          else {
              Firebase.setInt(firebaseData, "/Lock", 1);
              Firebase.setInt(firebaseData, "/LockBot", 1);
              //digitalWrite(LOCK, HIGH);
              myServo.write(180); // Locked position
              lockState = 1;
              lcd.setCursor(0, 1);
              lcd.print("  Lock Closed!  ");
              delay(2000);
            }
              BotFlag = false; // Reset the flag after processing
        }
        else {
            lcd.print(" Access Denied! ");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Access Denied! ");
            Firebase.setInt(firebaseData, "/Photo", 1);
            delay(2000);
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   SmartLock!   ");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}
