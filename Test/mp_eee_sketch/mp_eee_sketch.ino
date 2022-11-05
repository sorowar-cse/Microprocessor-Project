#include<LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define echo A0    //Echo pin
#define trigger A1 //Trigger pin

#define Recheck A5 // choose the pin for the Recheck button

#define G_led    8 // choose the pin for the Green Led
#define R_led    9 // choose the pin for the Red Led
#define buz  10 // choose the pin for the buz

// defines variables
float Speed = 0, distance = 0, Time = 0;
int flag1 = 0, flag2 = 0;
int time1, time2;

// Other configuration
const unsigned int BEEP_FREQUENCY = 3000;

void setup() {
  pinMode(Recheck, INPUT_PULLUP);

  pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
  pinMode(trigger, OUTPUT); // declare ultrasonic sensor Trigger pin as Output

  pinMode(R_led, OUTPUT); // declare Red LED as output
  pinMode(G_led, OUTPUT); // declare Green LED as output
  pinMode(buz, OUTPUT);  // declare buz as output

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Speed Test ");
  lcd.setCursor(0, 1);
  lcd.print("  Team Bratva  ");
  delay(2000);
  lcd.clear();
}


void loop() {
  ultrasonicRead();
  //  if (digitalRead (ir_s1) == LOW && flag1 == 0) {
  //    timer1 = millis();
  //    flag1 = 1;
  //  }
  //
  //  if (digitalRead (ir_s2) == LOW && flag2 == 0) {
  //    timer2 = millis();
  //    flag2 = 1;
  //  }

  if (distance <= 30 && flag1 == 0) {
    time1 = millis();
    flag1 = 1;
  }
  if (distance <= 20 && flag2 == 0) {
    time2 = millis();
    flag2 = 1;
  }

  if (flag1 == 1 && flag2 == 1) {
    if (time1 > time2) {
      Time = time1 - time2;
    }
    else if (time2 > time1) {
      Time = time2 - time1;
    }

    Time = Time / 1000; //convert millisecond to second
    Speed = (distance / Time); //v=d/t
    Speed = Speed * 3600; //multiply by seconds per hr
    Speed = Speed / 1000; //division by meters per Km
  }

  if (Speed == 0) {
    lcd.setCursor(0, 1);
    if (flag1 == 0 && flag2 == 0) {
      lcd.print("No car  detected");
    }
    else {
      lcd.print("Searching...    ");
    }
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed Km/Hr ");
    lcd.print(Speed);
    lcd.setCursor(0, 1);
    if (Speed > 50) {
      lcd.print("  Over Speeding  ");
      digitalWrite(buz, HIGH);
    }
    else {
      lcd.print("  Normal Speed   ");
    }
    delay(3000);
    digitalWrite(buz, LOW);
//    Speed = 0;
//    flag1 = 0;
//    flag2 = 0;
  }

   if (digitalRead(Recheck) == 0) {
    Speed = 0;
    flag1 = 0, flag2 = 0;
    digitalWrite(R_led, 1);
    digitalWrite(G_led, 0);
     //new 3 line:
//    digitalWrite(buz, HIGH);
//    delay(3000);
//    digitalWrite(buz, LOW);
    //tone(buz, BEEP_FREQUENCY, 400);  // Beep when done
  }
}

void ultrasonicRead() {
  digitalWrite(trigger, LOW); //PULSE ___|---|___
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn(echo, HIGH);
  distance = time / 28.5 / 2;
}
