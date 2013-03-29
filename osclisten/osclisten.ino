#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>
#include <SoftEasyTransfer.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(16,17);
SoftEasyTransfer ET; 

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 12);
const int  LISTEN_PORT  = 1234;

OSCServer listener;

int nextSpeed = 0;
int nextDuration = 0;

struct SEND_DATA_STRUCTURE{
  int theSpeed;
  int theDuration;
};
SEND_DATA_STRUCTURE mydata;

void setup(){
  Serial.begin(9600);
  
  Ethernet.begin(mac,ip);
  listener.begin(LISTEN_PORT);
  listener.addCallback("/motor/setSpeed",&gotSetSpeed);
  listener.addCallback("/motor/setDuration",&gotSetDuration);
  listener.addCallback("/motor/cue",&gotCue);
  
  mySerial.begin(9600);
  ET.begin(details(mydata), &mySerial);
  
}
  
void loop(){
  if(listener.aviableCheck()>0){}
  delay(50);
}

void gotCue(OSCMessage *_mes){
  forwardCue();
}

void gotSetDuration(OSCMessage *_mes){
  int d = _mes->getArgInt32(0);
  forwardCueDuration(d);
}

void gotSetSpeed(OSCMessage *_mes){
  int s = _mes->getArgInt32(0);
  forwardCueSpeed(s);
}

void forwardCueSpeed(int s){
 Serial.print("Setting speed for next cue to: "); 
 Serial.println(s);
nextSpeed = s; 
}

void forwardCueDuration(int d){
 Serial.print("Setting duration for next cue to: "); 
 Serial.println(d); 
 nextDuration = d;
}

void forwardCue(){
  Serial.print("Cue: ");
  Serial.print(nextSpeed);
  Serial.print(" ");
  Serial.println(nextDuration);
  mydata.theSpeed = nextSpeed;
  mydata.theDuration = nextDuration;
  ET.sendData();
}
