#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "DHT.h"
#define DHTPIN 6        // DHT22 is connected to digital pin 4
#define DHTTYPE DHT22   // DHT22 type

DHT dht(DHTPIN, DHTTYPE);
const int mq135_aqi_sensor = A0;
// Set threshold for AQI
int aqi_ppm = 0;

const int ldrPin = 7;
const int irPin = 5;

const int pingPin = 4; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 3; // Echo Pin of Ultrasonic Sensor

// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const u1_t PROGMEM APPEUI[8]={0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
// LoRaWAN AppSKey, application session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const u1_t PROGMEM DEVEUI[8]= {0xD6, 0x1D, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// LoRaWAN end-device address (DevAddr)
static const u1_t PROGMEM APPKEY[16] = {0xF7, 0xBA, 0xA6, 0x21, 0xC7, 0x46, 0x40, 0xFF, 0x91, 0xE9, 0xD1, 0x3D, 0xE3, 0x27, 0x30, 0xD9};
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

uint8_t Data;
static uint8_t mydata[6] = {0x00,0x00,0x00};
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
const unsigned TX_INTERVAL = 30;

void HC04()
{
  long duration, cm;
   //pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();

   mydata[0] = cm;
  
  //delay(9000); //Delay of 1 second for ease of viewing
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void AQI_MQ135()
{
  aqi_ppm = analogRead(mq135_aqi_sensor);

  Serial.print("Air Quality: ");
  Serial.println(aqi_ppm);
  Serial.println();
  mydata[3] = aqi_ppm;
  
  //delay(9000); //Delay of 1 second for ease of viewing
}

void IR()
{
  int value = 1;
  if(digitalRead(irPin) == 0){
    value = 1;
    Serial.print("Object Detected!");
  }
  else{
    value = 0;
    Serial.print("No Object Detected!");
  }
  Serial.println();
  mydata[1] = value;
  
  //delay(9000); //Delay of 1 second for ease of viewing
}

void LDR()
{
  int value1 = 0;
  if(digitalRead(ldrPin) == 1){
    value1 = 1;
    Serial.print("Bright!");
  }
  else{
    value1 = 0;
    Serial.print("Dark!");
  }
  Serial.println();
  mydata[2] = value1;
  
  //delay(9000); //Delay of 1 second for ease of viewing
}

void Temp(){
  int humidity = dht.readHumidity();  // Read the humidity
  int temperature = dht.readTemperature();  // Read the temperature

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the temperature and humidity values to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C");
  Serial.print("\tHumidity: ");
  Serial.print(humidity);
  Serial.println("%");

  mydata[4] = temperature;
  mydata[5] = humidity;
}

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              for (int i = 0; i < LMIC.dataLen; i++) 
              {
               if (LMIC.frame[LMIC.dataBeg + i] < 0x10) 
               {
                 Data = (LMIC.frame[LMIC.dataBeg + i]);
               }
              }
              Serial.print("Downlink Data : ");
              Serial.println(Data);
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) 
    {
        Serial.println(F("OP_TXRXPEND, not sending"));
    }
    else 
    {
        HC04();
        AQI_MQ135();
        IR();
        LDR();
        Temp();

        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata), 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void setup() {
    pinMode(pingPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(mq135_aqi_sensor, INPUT);
    pinMode(irPin, INPUT);
    pinMode(ldrPin, INPUT);
    dht.begin();          // Initialize the DHT22 sensor
    Serial.begin(9600);
    Serial.println(F("Starting"));

    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7,14);

    // Start job
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}
