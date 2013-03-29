#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 12);
const int  LISTEN_PORT  = 1234;

OSCServer listener;

void setup(){
  Serial.begin(9600);
  
  Ethernet.begin(mac,ip);
  listener.begin(LISTEN_PORT);
  listener.addCallback("/motor/setSpeed",&gotSetSpeed);
  listener.addCallback("/motor/setDuration",&gotSetDuration);
  listener.addCallback("/motor/cue",&gotCue);
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
}

void forwardCueDuration(int d){
 Serial.print("Setting duration for next cue to: "); 
 Serial.println(d); 
}

void forwardCue(){
  Serial.println("Cue!"); 
}
