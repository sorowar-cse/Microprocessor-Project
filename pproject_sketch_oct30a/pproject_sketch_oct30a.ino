#include<LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define echo A0
#define trigger A1
#define Recheck A5
#define B_led 8
#define R_led 9
#define buz 10
float Speed = 0, distance = 0, Time = 0;
int flag1 = 0, flag2 = 0;
int time1, time2;
const unsigned  int BEEP_FREQUENCY = 3000;
void setup() {
  pinMode(Recheck, INPUT_PULLUP);
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(R_led, OUTPUT);
  pinMode(B_led, OUTPUT);
  pinMode(buz, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Vehicle Speed ");
  lcd.setCursor(0, 1);
  lcd.print(" Measurement ");
  delay(5000);
  lcd.clear();
}

void loop() {
  ultrasonicRead();
  if (distance <= 30 && flag1 == 0)
  { 
    time1 = millis();
    flag1 = 1;
  }
  if (distance <= 20 && flag2 == 0)
  { time2 = millis();
    flag2 = 1;
  }
  if (flag1 == 1 && flag2 == 1)
  {
    flag1 = 2,
    flag2 = 2;
  }
  if (time1 > time2)
  {
    Time = time1 - time2;
  }
  else if (time2 > time1)
  {
    Time = time2 - time1;
  }
  Time = Time / 1000;
  Speed = (10 / Time);
  Speed = Speed * 3600;
  Speed = Speed / 1000;
  digitalWrite(R_led, 0);
  digitalWrite(B_led, 1);
  tone(buz, BEEP_FREQUENCY, 200);
  if (digitalRead(Recheck) == 0) {
    Speed = 0;
    flag1 = 0, flag2 = 0;
    digitalWrite(R_led, 1);
    digitalWrite(B_led, 0);
    tone(buz, BEEP_FREQUENCY, 400);
    lcd.setCursor(0, 0);
    lcd.print("Dist.=");
    lcd.print(distance);



    lcd.print(" cm ");
    lcd.setCursor(0, 1);
    lcd.print(" Speed=");
    lcd.print(Speed);
    lcd.print("Km/h ");
    delay(10);
  }
}

void ultrasonicRead() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn(echo, HIGH);
  distance = time * 0.034 / 2;
}
