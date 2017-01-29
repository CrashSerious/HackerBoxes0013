

// the start of this code was taken from a comment from "mainegeek" in the HackerBox0013 instructable: http://www.instructables.com/id/HackerBoxes-0013-Autosport/

#include <ESP8266WiFi.h>
#include <SimpleTimer.h>
//#include <BlynkSimpleEsp8266.h>
#define RightMotorSpeed 5
#define RightMotorDir 0
#define LeftMotorSpeed 4
#define LeftMotorDir 2
#define ECHO_PIN 12
#define TRIG_PIN 13
#define LED_PIN 16


#define Move 1 // for debugging, without driving all over the country side

SimpleTimer timer;

long ObjectAhead;
int SlowDelay = 1000;
int FastDelay = 1000;
int Deciding;
int MinDistance = 10;

void setup()

{

  Serial.begin(9600);
    
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
  
  timer.setInterval(100, sendDistanceData);
}

void sendDistanceData()
{
  ObjectAhead = getDistance();
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  Serial.println(duration/58.2);
  return duration/58.2;
}

void loop()
{
  Serial.println(ObjectAhead);
  if (ObjectAhead>MinDistance)
  {
    if (ObjectAhead > 100){
      MinDistance = 10;
    }
    forward();
    if (Deciding>1){
      Deciding--;
    }
    //delayMicroseconds(250);
  }
  else
  {
    decide();
    //delayMicroseconds(500);   
  }
  
  timer.run();
}

void decide()
{
  
  if (Deciding < 5) {
    MinDistance= 20;
    reverse();
    Deciding++;
  }
  else
  {
    Deciding--;
    reverse();
    //delayMicroseconds(500);
    if ((rand()*100) > 50)
      {
        Serial.print ("Deciding: ");
        Serial.print (Deciding);
        Serial.print (" --> ");
        left();
        //delayMicroseconds(10000);
      }
      else
      {
        Serial.print ("Deciding: ");
        Serial.print (Deciding);
        Serial.print (" --> ");
        right();
        //delayMicroseconds(10000);
      }
  }
}

void MoveViaPin(int ThePin, bool TheState)
{
  digitalWrite(ThePin, TheState);
/*  if (TheState)
  {
    delayMicroseconds(FastDelay);
  }
  else
  {
    delayMicroseconds(SlowDelay);
  }*/
}

void halt()

{
  Serial.println("Halting! ");
  digitalWrite(RightMotorSpeed, LOW);
  digitalWrite(LeftMotorSpeed, LOW);
}



void forward(){
  Serial.println("Moving forward!");
  if (Move){
    MoveViaPin(RightMotorDir, LOW);
    MoveViaPin(LeftMotorDir, HIGH);
    MoveViaPin(RightMotorSpeed, HIGH);
    MoveViaPin(LeftMotorSpeed, HIGH);    
  }

}

void reverse(){
  Serial.println("Moving reverse!");
  if (Move){
    MoveViaPin(RightMotorDir, HIGH);
    MoveViaPin(LeftMotorDir, LOW);
    MoveViaPin(RightMotorSpeed, HIGH);
    MoveViaPin(LeftMotorSpeed, HIGH);
   }
}

void right(){
  Serial.println("Moving right!");
  if (Move){
    MoveViaPin(RightMotorDir, LOW);
    MoveViaPin(RightMotorSpeed, HIGH);    
  }
}

void left(){
  Serial.println("Moving left!");
  if (Move){
    MoveViaPin(LeftMotorDir, HIGH);
    MoveViaPin(LeftMotorSpeed, HIGH);
  }

}
