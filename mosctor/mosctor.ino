#include "DualVNH5019MotorShield.h"

// MOTOR SHIELD

const unsigned char INA1 = 2;
const unsigned char INB1 = 4;
const unsigned char EN1DIAG1 = 6;
const unsigned char CS1 = A0;
const unsigned char INA2 = 7;
const unsigned char INB2 = 8;
const unsigned char EN2DIAG2 = 12;
const unsigned char CS2 = A1;

DualVNH5019MotorShield md = DualVNH5019MotorShield(INA1,INB1,EN1DIAG1,CS1,INA2,INB2,EN2DIAG2,CS2);

// MISC OTHER PINS

const int BRAKEPIN = 3;

// SET VIA OSC

int nextCueSpeed = 100;
int nextCueDuration = 2000;

// STATE VARIABLES

unsigned long cueEndTime;
boolean cueRunning = false;
boolean shouldCue = false;

////////////////////////////////////////////////////////////// ARDUINO LIFECYCLE

void setup() {
  Serial.begin(9600);
  md.init();
  pinMode(BRAKEPIN,OUTPUT);
}

void loop(){
  cueCheckEnd();
  if (shouldCue){
   shouldCue= false;
    cue(); 
  }
  delay(1000);
}

////////////////////////////////////////////////////////////// MOTOR CONTROL

void stopIfFault() {
  if (md.getM1Fault()){
    Serial.println("M1 fault");
    while(1);
  }
}

void brakeOff(){
  digitalWrite(3,HIGH);
}

void brakeOn(){
  digitalWrite(3,LOW);
}

void cueCheckEnd(){
  if (cueRunning){
    if (millis() > cueEndTime){
        cueEnd();
        delay(10000);
      }   
  }
}

void cueEnd(){
   brakeOn(); 
   md.setM1Speed(0);
   cueRunning = false;
}

void cue(){
    Serial.print("Got cue: will drive at ");
    Serial.print(nextCueSpeed);
    Serial.print(" for ");
    Serial.print(nextCueDuration);
    Serial.println(" milliseconds");
    cueRunning = true;
    cueEndTime = millis() + nextCueDuration;
    brakeOff();
    md.setM1Speed(nextCueSpeed);
}

////////////////////////////////////////////////////////////// COMMS

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if (inChar == 'c') {
      shouldCue = true;
    } 
  }
}
