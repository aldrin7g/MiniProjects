int sensor = 8;
int led = 9;
int buzzer = 10;
int relay = 11;
int rst = 3;

bool detect = false;
bool reset = false;

void setup() {
pinMode(sensor, INPUT);
pinMode(rst, INPUT);
pinMode(led, OUTPUT);
pinMode(buzzer, OUTPUT);
pinMode(relay, OUTPUT);
Serial.begin(9600);
}

void loop() {
if((digitalRead(sensor) == LOW)||(detect == true)){
  detect = true;
  if ((digitalRead(rst) == true)){
    detect = false;
  }
  digitalWrite(led,HIGH);
  digitalWrite(buzzer, HIGH);
  digitalWrite(relay, LOW);
  }
else{
  digitalWrite(led,LOW);
  digitalWrite(buzzer,LOW);
  digitalWrite(relay,HIGH);
}
delay(100);
}