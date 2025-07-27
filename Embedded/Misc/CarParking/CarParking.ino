//#include <LiquidCrystal.h>// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
#include <Servo.h> //includes the servo library
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo1;

int ir_s1 = 14;
int ir_s2 = 12;

int Total = 5;
int Space;

int flag1 = 0;
int flag2 = 0;

void setup() {
Wire.begin();
lcd.init();
lcd.backlight();
pinMode(ir_s1, INPUT);
pinMode(ir_s2, INPUT);
  
myservo1.attach(16);
myservo1.write(180);

lcd.begin(16, 2);  
lcd.setCursor (0,0);
lcd.print("  Car  Parking  ");
lcd.setCursor (0,1);
lcd.print("     System     ");
delay (2000);
lcd.clear();  

Space = Total;
}

void loop(){ 

if(digitalRead (ir_s1) == LOW && flag1==0){
if(Space>0){flag1=1;
if(flag2==0){myservo1.write(0); Space = Space-1;}
}else{
lcd.setCursor (0,0);
lcd.print(" Sorry not Space ");  
lcd.setCursor (0,1);
lcd.print("    Available    "); 
delay (1000);
lcd.clear(); 
}
}

if(digitalRead (ir_s2) == LOW && flag2==0){flag2=1;
if(flag1==0){myservo1.write(0); Space = Space+1;}
}

if(flag1==1 && flag2==1){
delay (1000);
myservo1.write(180);
flag1=0, flag2=0;
}

lcd.setCursor (0,0);
lcd.print("Total Space: ");
lcd.print(Total);

lcd.setCursor (0,1);
lcd.print("Have  Space: ");
lcd.print(Space);
}