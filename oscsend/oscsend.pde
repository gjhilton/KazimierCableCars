import oscP5.*;
import netP5.*;
import controlP5.*;

ControlP5 cp5;

OscP5 oscP5;
NetAddress myRemoteLocation;

void setup() {
  //frameRate(10);
  size(300, 140);
 
 oscP5 = new OscP5(this,12000);
 myRemoteLocation = new NetAddress("192.168.1.12",1234);
 
 
 
cp5 = new ControlP5(this);
cp5.addSlider("speed")
     .setPosition(5,5)
     .setSize(200,20)
     .setRange(-200,200)
     .setValue(0)
     ;

cp5.addSlider("duration")
     .setPosition(5,35)
     .setSize(200,20)
     .setRange(1000,10000)
     .setValue(1000)
     ;

cp5.addBang("bang")
     .setPosition(5, 70)
     .setSize(20, 20)
     .setLabel("cue")
     ;

}

void draw() {
  background(50);
}

void speed(float v) {
  int i = floor(v);
  OscMessage myMessage = new OscMessage("/motor/setSpeed");
  myMessage.add(i); 
  oscP5.send(myMessage, myRemoteLocation);
}

void duration(float v) {
  int i = floor(v);
  OscMessage myMessage = new OscMessage("/motor/setDuration");
  myMessage.add(i); 
  oscP5.send(myMessage, myRemoteLocation);
}

public void bang() {
  OscMessage myMessage = new OscMessage("/motor/cue");
  oscP5.send(myMessage, myRemoteLocation);
}
