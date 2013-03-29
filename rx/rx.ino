#include <SoftEasyTransfer.h>
#include <SoftwareSerial.h>
#include "DualVNH5019MotorShield.h"

SoftwareSerial mySerial(16,17);
SoftEasyTransfer ET; 
struct RECEIVE_DATA_STRUCTURE{
  int theSpeed;
  int theDuration;
};
RECEIVE_DATA_STRUCTURE mydata;

DualVNH5019MotorShield md;

const int BRAKEPIN = 3;

// STATE VARIABLES

unsigned long cueEndTime;
boolean cueRunning = false;
boolean shouldCue = false;

void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);
  ET.begin(details(mydata), &mySerial); 
  md.init();
  pinMode(BRAKEPIN,OUTPUT);
}

void loop(){
  cueCheckEnd();
  if(ET.receiveData()){
    Serial.println("Got cue");
    cue(mydata.theSpeed, mydata.theDuration);
  }
  delay(250);
}

void cue(int s, int d){
  Serial.print("Got cue: will drive at ");
    Serial.print(s);
    Serial.print(" for ");
    Serial.print(d);
    Serial.println(" milliseconds");
  cueRunning = true;
  cueEndTime = millis() + d;
  brakeOff();
  md.setM1Speed(s);
}

void cueEnd(){
   brakeOn(); 
   md.setM1Speed(0);
   cueRunning = false;
}

void cueCheckEnd(){
  if (cueRunning){
    if (millis() > cueEndTime){
        cueEnd();
      }   
  }
}

void brakeOff(){
  digitalWrite(3,HIGH);
}

void brakeOn(){
  digitalWrite(3,LOW);
}
