#include <Servo.h>
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#define PHONE_NUMBER "01000000000" //Enter Mobile Number
#define MESSAGE  "Turn off the gas stove knob"
DFRobot_SIM808 sim808(&Serial);
Servo myservo1, myservo2;

boolean flag1 = false;
int count = 1;
int sensorThres = 100;
char state;
int buzzer = 13;
int lpgA0 = A0;
int human = -1;
int fire = -1;

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(lpgA0, INPUT);
  myservo1.attach(6);
  myservo2.attach(9);
  myservo1.write(90);
  myservo2.write(90);
}
void loop() {
  if (Serial.available()) {
    state = Serial.read();
    Serial.println(state);
    Serial.flush();
  }
  switch (state) {
    case 's':
      human = 1;
      break;
    case 't':
      fire = 1;
      break;
    case 'x':
      human = 0;
      fire = 0;
      flag1 = true;
  }
  int analogSensor = analogRead(lpgA0);
  if (analogSensor > sensorThres) {
    switch (human) {
      case 1:
        alarm(); 
        count = 1;
        break;
      case 0:
        myservo1.write(180);   
        digitalWrite(buzzer, LOW);
        gsm();
        break;
    }
    switch (fire) {
      case 1:
        Serial.println("IN FIRE!!!!");
        //digitalWrite(buzzer, LOW);
        myservo1.write(180);
        myservo2.write(180);
        count = 1;
        break;
      case 0:
        digitalWrite(buzzer, LOW);
    }
  }
}
void alarm() {
  digitalWrite(buzzer, HIGH);
  Serial.println("alarm");
}
void gsm() {
  Serial.println("in gsm");
  if (flag1 == true) {
    if (count == 1) {
      sim808.sendSMS(PHONE_NUMBER, MESSAGE);
      Serial.println("send sms");
      Serial.println(sim808.sendSMS(PHONE_NUMBER, MESSAGE));
      flag1 = false;
      count = count + 1;
    }
  }
}
