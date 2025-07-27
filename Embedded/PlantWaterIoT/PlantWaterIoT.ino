#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3cXt2JJgF"
#define BLYNK_TEMPLATE_NAME "PlantWater"
#define BLYNK_AUTH_TOKEN "ju6otSTI4EzNQLhTD3VfxEeWxEAbRtVc"
#include <LiquidCrystal_I2C.h> // Driver Library for the LCD Module
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2); // Adjust to (0x27,20,4) for 20x4 LCD

#define PUMP_PIN 14        // Pin connected to relay module for controlling the pump
#define TRIGGER_PIN 13     // Ultrasonic sensor trigger pin
#define ECHO_PIN 12        // Ultrasonic sensor echo pin
#define MOISTURE_PIN A0   // Analog pin for soil moisture sensor
#define MOISTURE_PIN1 16   // Analog pin for soil moisture sensor

const int maxWaterLevel = 30; // Maximum water level in the tank (in centimeters)
const int sensorHeight = 3; // Maximum water level in the tank (in centimeters)
//const int moistureThreshold = 500; // Threshold value for soil moisture (adjust as needed)
const int pumpRunTime = 5000; // Time in milliseconds to run the pump
int value = 0; int moistureLevel = 0; long distance = 0;
// This function sends Arduino's up time every second to Virtual Pin (5)

BLYNK_WRITE(V2)
{
  value = param.asInt(); // assigning incoming value from pin V1 to a variable
}

void sendSensor(){
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, moistureLevel);
  Blynk.virtualWrite(V0, distance);
}

void relay(){
  Blynk.virtualWrite(V2, value);
}

void setup()
{
  Serial.begin(115200);
  // Initiate the LCD and turn on the backlight
  lcd.init();          // Initiate the LCD module
  lcd.backlight();     // Turn on the backlight
  Blynk.begin(BLYNK_AUTH_TOKEN, "Virus#404", "password1");
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(100L, relay);
  pinMode(PUMP_PIN, OUTPUT);   // Set pump pin as output
  pinMode(TRIGGER_PIN, OUTPUT); // Set ultrasonic sensor trigger pin as output
  pinMode(ECHO_PIN, INPUT);     // Set ultrasonic sensor echo pin as input
  digitalWrite(PUMP_PIN, HIGH);
  // Print 'Hello World!' on the first line of the LCD
  lcd.setCursor(4, 0);          // Set the cursor on the first column and first row.
  lcd.print("!WELCOME!");    // Print the string "Hello World!"
  lcd.setCursor(5, 1);          // Set the cursor on the first column and first row.
  lcd.print("Aldrin");
  delay(5000);
  lcd.clear();
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  // Measure water level using ultrasonic sensor
  long duration;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = maxWaterLevel - (((duration / 2) / 29.1)-sensorHeight); // Subtract distance from max water level
  if(distance < 0 || distance >30)
    distance = 0;
  distance = map(distance, 0, maxWaterLevel, 0, 100);
  // Read soil moisture level
  moistureLevel = analogRead(MOISTURE_PIN);
  moistureLevel = map(moistureLevel, 1023, 0, 0, 100);
  int moisture  = digitalRead(MOISTURE_PIN1);
  
  Serial.print("\nMoisture: ");
  Serial.print(moistureLevel);
  Serial.println("%");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil : ");
  lcd.print(moistureLevel);
  lcd.print("%");

  Serial.print("Water: ");
  Serial.print(distance);
  Serial.println("%");
  lcd.setCursor(0, 1);
  lcd.print("Water: ");
  lcd.print(distance);
  lcd.print("%");
  Serial.print("Button: ");
  Serial.println(value);
  Serial.print("Sensor: ");
  Serial.println(moisture);
  //Serial.print(distance);
  sendSensor();
  relay();
  
  
  lcd.setCursor(12, 0);
  lcd.print("PUMP");
  lcd.setCursor(12, 1);
  lcd.print("OFF!");

  // Check conditions to run the pump
  if ((distance > 5 && moisture == 1) || (value == 1)) {
    // Water level is above threshold and soil moisture is below threshold
    // Turn on the pump
    value = 1;
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("Pump ON");
    lcd.setCursor(12, 1);
    lcd.print(" ON ");
    relay();
    // Run the pump for specified time
    delay(pumpRunTime);

    // Turn off the pump
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("Pump OFF");
    lcd.setCursor(12, 1);
    lcd.print("OFF!");
    value = 0;
    relay();
  }
  delay(1000); // Delay for stability
}