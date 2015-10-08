#include <Servo.h>

//Micro
const int calibPin0 =  A6; //set the MAX value of the sensor input
const int calibPin1 =  A5; //set the MIN value of the sensor input
const int thumbPin =  A4; //change the thumb position among three preset values
const int fingerPin =  A3; //lock or unlock the position of middle finger, ring finger and pinky
const int analogInPin0 = A0; //sensor input

Servo myservo0; //controls index finger
Servo myservo1; //controls middle finger, ring finger and pinky
Servo myservo2; //controls thumb

float target = 0;
boolean thumbPinState = 1;
boolean fingerPinState = 1;

int count = 0;
int mode = 0;
int val = 0;

int swCount0 = 0;
int swCount1 = 0;
int swCount2 = 0;
int swCount3 = 0;

int sensorValue = 0; // value read from the sensor
int sensorMax = 700;
int sensorMin = 0;
int threshold = 0;

//speed settings
int speedMax = 7;
int speedMin = 0;
int speedReverse = -3;
int speed = 0;

int positionMax = 150;
int positionMin = 0;
int position =0;
int prePosition = 0;

int thumbPinch = 120;  
int thumbOpen = 50;


int indexMin = 0;//extend
int indexMax = 160;//flex

int middleMin = 65;//extend 
int middleMax = 130;//flex

int thumbPos = 90;
int indexPos = 90;
int middlePos = 90;

void setup() {
  Serial.begin(9600); 

  pinMode(calibPin0, INPUT); //MAX A6
  digitalWrite(calibPin0, HIGH);
  
  pinMode(calibPin1, INPUT); //MIN A5
  digitalWrite(calibPin1, HIGH);
  
  pinMode(thumbPin, INPUT); //A4
  digitalWrite(thumbPin,HIGH);

  pinMode(fingerPin, INPUT); //A3
  digitalWrite(fingerPin,HIGH);

  myservo0.attach(3);//index  
  myservo1.attach(5);//middle
  myservo2.attach(6);//thumb
}

void loop() { 
  sensorValue = ReadSens_and_Condition();
  delay(25);
    
  if(digitalRead(calibPin0) == LOW){//A6
     swCount0 += 1;
  }
  else{
    swCount0 = 0;
  }
  
  if(swCount0 == 10){
    swCount0 = 0;
    sensorMax = ReadSens_and_Condition();    
  }
  
  if(digitalRead(calibPin1) == LOW){//A5
     swCount1 += 1;
  }
  else{
    swCount1 = 0;
  }
  
  if(swCount1 == 10){
    swCount1 = 0;
    sensorMin = ReadSens_and_Condition() + threshold;
  }

  if(digitalRead(thumbPin) == LOW){//A4
     swCount2 += 1;
  }
  else{
    swCount2 = 0;
  }

  if(swCount2 == 10){
    swCount2 = 0;
    thumbPinState = !thumbPinState;    
    while(digitalRead(thumbPin) == LOW){delay(1);}    
    }
  

  if(digitalRead(fingerPin) == LOW){//A3
     swCount3 += 1;
  }
  else{
    swCount3 = 0;
  }
  if(swCount3 == 10){
    swCount3 = 0;
    fingerPinState = !fingerPinState;
    while(digitalRead(fingerPin) == LOW){delay(1);}    
  }


//status
  Serial.print("Min=");
  Serial.print(sensorMin);
  Serial.print(",Max=");
  Serial.print(sensorMax);
  Serial.print(",Value=");
  Serial.print(sensorValue);
  Serial.print(",thumb=");
  Serial.print(thumbPinState);
  Serial.print(",finger=");
  Serial.print(fingerPinState);  
  Serial.print(",indexPos=");
  Serial.print(indexPos);  
  Serial.print(",thumb=");
  Serial.print(swCount3);
  Serial.print(",speed=");
  Serial.print(speed);
  
  Serial.print(",thumbPinState=");
  Serial.print(thumbPinState);
  Serial.print(",fingerPinState=");
  Serial.println(fingerPinState);  
  
//calculate speed
  if(sensorValue < (sensorMin+(sensorMax-sensorMin)/8)){
    speed = speedReverse;
  }
  else if(sensorValue < (sensorMin+(sensorMax-sensorMin)/4)){
    speed = 0;
  }
  else{
    speed = map(sensorValue, sensorMin, sensorMax, speedMin, speedMax);
  }

//calculate position
  position = prePosition + speed;
  if(position < positionMin) position = positionMin;
  if(position > positionMax) position = positionMax;
  prePosition = position;
 //motor
  indexPos=map(position,positionMin,positionMax,indexMin, indexMax);
  
  myservo0.write(indexPos);

  if(fingerPinState == HIGH){
     middlePos=map(position,positionMin,positionMax,middleMin, middleMax);
     myservo1.write(middlePos);
  }

  switch(thumbPinState){
    case 0://pinch
      myservo2.write(thumbPinch);
      break;
    case 1://open
      myservo2.write(thumbOpen);
      break;
    default:
      break;
  }
}


int ReadSens_and_Condition(){
  int i;
  int sval;
  for(i= 0; i<20; i++){
    sval = sval + analogRead(analogInPin0);
  }
  sval = sval/20;
  return sval;
}

