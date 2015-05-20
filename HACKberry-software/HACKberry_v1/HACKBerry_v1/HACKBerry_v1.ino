#include <avr/interrupt.h>  // 外部割り込みライブラリのインクルード
#include <avr/sleep.h>
#include <Servo.h>

////Micro
boolean calibPin0 =  A6;      // the number of the calibration pin for MAX
boolean calibPin1 =  A5;      // the number of the calibration pin for MIN
boolean thumbPin =  A4;      // 
boolean fingerPin =  A3;      //
const int analogInPin0 = A0;

Servo myservo0;
Servo myservo1;
Servo myservo2;

float target = 0;
int thumbPinState = 1;
int fingerPinState = 1;

int count = 0;
int mode = 0;
int val = 0;

int swCount0 = 0;  //チャタリング防止用のカウンタ
int swCount1 = 0;  //チャタリング防止用のカウンタ
int swCount2 = 0;  //チャタリング防止用のカウンタ
int swCount3 = 0;  //チャタリング防止用のカウンタ

int sensorValue0 = 0;        // value read from the sensor
int sensorValue1 = 0;        // value read from the sensor

int sensorMax = 700;
int sensorMin = 0;
int threshold = 0; //ミニマム側の底上げ

//speed settings
boolean inputState = HIGH;
boolean prevInputState = HIGH;
int speedMax = 5;
int speedMin = 0;
int speedInverse = -1;
int speed = 0;

int positionMax = 100;
int positionMin = 0;
int position =50;
int prePosition = 50;

int thumbGrasp = 45;  
int thumbPinch = 58;  
int thumbOpen = 153;


int indexMin = 180;//extend
int indexMax = 0;//flex
/*DM1500可動域 150-*/
int middleMin = 125
;//extend 
int middleMax = 40;//flex

int thumbPos = 90;
int indexPos = 90;
int middlePos = 90;

void setup() {
  Serial.begin(9600);      // モニタ用にメッセージを出力

  pinMode(calibPin0, INPUT);  // MAX
  digitalWrite(calibPin0, HIGH);//プルアップ
  
  pinMode(calibPin1, INPUT);  // MIN
  digitalWrite(calibPin1, HIGH);//プルアップ
  
  pinMode(thumbPin, INPUT);  // MIN
  digitalWrite(thumbPin,HIGH);//プルアップ

  pinMode(fingerPin, INPUT);  // MIN
  digitalWrite(fingerPin,HIGH);//プルアップ

  myservo0.attach(3);//index  
  myservo1.attach(5);//middle
  myservo2.attach(6);//thumb
}

void loop() { 
  sensorValue0 = ReadSens_and_Condition();
  delay(10);
    
  //センサ読み取り値のキャリブレーション 
  if(digitalRead(calibPin0) == LOW){//A6が押された場合
     swCount0 += 1;
  }
  else{
    swCount0 = 0;
  }
  
  if(swCount0 == 20){
    swCount0 = 0;
    sensorMax = ReadSens_and_Condition();    
  }
  
  if(digitalRead(calibPin1) == LOW){//A5が押された場合
     swCount1 += 1;
  }
  else{
    swCount1 = 0;
  }
  
  if(swCount1 == 20){
    swCount1 = 0;
    sensorMin = ReadSens_and_Condition() + threshold;//脱力時に完全に静止させるため
  }

  if(digitalRead(thumbPin) == LOW){//A4が押された場合
     swCount2 += 1;
  }
  else{
    swCount2 = 0;
  }

  if(swCount2 == 10){
    swCount2 = 0;
    thumbPinState ++;
    if(thumbPinState > 2){
      thumbPinState = 0;    
    }
    while(digitalRead(thumbPin) == LOW){delay(1);}   
  }

  if(digitalRead(fingerPin) == LOW){//A3が押された場合
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


//ステータス
  Serial.print("Min=");
  Serial.print(sensorMin);
  Serial.print(",Max=");
  Serial.print(sensorMax);
  Serial.print(",Value=");
  Serial.print(sensorValue0);
  Serial.print(",thumb=");
  Serial.print(thumbPinState);
  Serial.print(",finger=");
  Serial.print(fingerPinState);  
  Serial.print(",indexPos=");
  Serial.print(indexPos);
  Serial.print(",speed=");
  Serial.println(speed);
  
  

//calculate speed
  inputState = HIGH;
  if(sensorValue0 < (sensorMin+(sensorMax-sensorMin)/8)){
    speed = speedInverse;
    if(inputState == LOW) inputState = HIGH;
  }
  else if(sensorValue0 < (sensorMin+(sensorMax-sensorMin)/4)){
    speed = 0;
  }
  else{
    speed = map(sensorValue0, sensorMin, sensorMax, speedMin, speedMax);
      if(inputState == HIGH) inputState = LOW;
  }
  prevInputState = inputState;

  //calculate position
  position = prePosition + speed;
  if(position < positionMin) position = positionMin;
  if(position > positionMax) position = positionMax;
  prePosition = position;

  //モーターへの命令
  indexPos=map(position,positionMin,positionMax,indexMin, indexMax);
  middlePos=map(position,positionMin,positionMax,middleMin, middleMax);

  myservo0.write(indexPos);

  if(fingerPinState == HIGH){
  myservo1.write(middlePos);
  }


  switch(thumbPinState){
    case 0://grasp
      myservo2.write(thumbGrasp);
      break;
    case 1://pinch
      myservo2.write(thumbPinch);
      break;
    case 2://open
      myservo2.write(thumbOpen);
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

