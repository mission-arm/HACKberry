/*
    Arduino micro code for HACKberry.
    Origially created by exiii Inc.
    edited by Genta Kondo on 2017/6/11
*/
#include <Servo.h>

//Settings
const boolean isRight = 1;//right:1, left:0
int speed = 4;  //指の開閉速度を変更する場合はこの変数を変更する

//For right hand, find optimal values of ThumbMin, IndexMax and OtherMax first.
//For left hand, find optimal values of ThumbMax, IndexMin and OtherMin first.
//Then, calculate the remaining values by following rules.
//Difference of ThumbMin and ThumbMax is 86
//Difference of IndexMin and IndexMax is 117
//Difference of OtherMin and OtherMax is 55

const int outThumbMax = 140;//right:open, left:close
const int outIndexMax = 140;//right:open, left:close
const int outOtherMax = 120;//right:open, left:close

const int outThumbMin = 140-86;//right:close, left:open
const int outIndexMin = 140-117; //right:close, left:open
const int outOtherMin = 120-55;//right:close, left:open

const int speedMax = 6;
const int speedMin = 0;
const int speedReverse = -3;
const int thSpeedReverse = 15;//0-100
const int thSpeedZero = 30;//0-100
const boolean onSerial = 0;   //Mk2 doesn't use serial monitor

//Hardware
Servo servoIndex; //index finger
Servo servoOther; //other three fingers
Servo servoThumb; //thumb
int pinCalib; //start calibration
//int pinTBD;
int pinThumb; // open/close thumb
int pinOther; //lock/unlock other three fingers
int pinSensor = A1; //sensor input
int pinSensor2 = A2;

//Software
boolean isThumbOpen = 1;
boolean isOtherLock = 0;
int swCount0, swCount1, swCount2, swCount3 = 0;
int sensorValue = 0; // value read from the sensor
int sensorValue2 = 0;
int sensorMax = 700;
int sensorMin = 0;
int sensorMax2 = 700;
int sensorMin2 = 0;
int position = 0;
const int positionMax = 100;
const int positionMin = 0;
int prePosition = 0;
int outThumb, outIndex, outOther = 90;
int outThumbOpen, outThumbClose, outIndexOpen, outIndexClose, outOtherOpen, outOtherClose;

void setup() {
  Serial.begin(9600);
  if (isRight) {
    pinCalib =  A6;
    //pinTBD =  A7;
    pinThumb =  A0;
    pinOther =  10;
    outThumbOpen = outThumbMax; outThumbClose = outThumbMin;
    outIndexOpen = outIndexMax; outIndexClose = outIndexMin;
    outOtherOpen = outOtherMax; outOtherClose = outOtherMin;
  }
  else {
    pinCalib =  A6;
    //pinTBD =  A7;
    pinThumb =  A0;
    pinOther =  10;
    outThumbOpen = outThumbMin; outThumbClose = outThumbMax;
    outIndexOpen = outIndexMin; outIndexClose = outIndexMax;
    outOtherOpen = outOtherMin; outOtherClose = outOtherMax;
  }
  servoIndex.attach(5);//index
  servoOther.attach(6);//other
  servoThumb.attach(9);//thumb
  //pinMode(pinCalib, INPUT);//A6
  //digitalWrite(pinCalib, HIGH);
  //pinMode(pinTBD, INPUT);//A5
  //digitalWrite(pinTBD, HIGH);
  pinMode(pinThumb, INPUT);//A4
  digitalWrite(pinThumb, HIGH);
  pinMode(pinOther, INPUT);//A3
  digitalWrite(pinOther, HIGH);
}

void loop() {
  //==waiting for calibration==
  if (onSerial) Serial.println("======Waiting for Calibration======");
  while (1) {
    servoIndex.write(outIndexOpen);
    servoOther.write(outOtherOpen);
    servoThumb.write(outThumbOpen);
    if (onSerial) serialMonitor();
    delay(10);
    if (DigitalRead(pinCalib) == LOW) {
      calibration();
      break;
    }
  }
  //==control==
  position = positionMin;
  prePosition = positionMin;
  while (1) {
    if (DigitalRead(pinCalib) == LOW) swCount0 += 1;
    else swCount0 = 0;
    if (swCount0 == 10) {
      swCount0 = 0;
      calibration();
    }
    if (digitalRead(pinThumb) == LOW) swCount2 += 1;
    else swCount2 = 0;
    if (swCount2 == 10) {
      swCount2 = 0;
      isThumbOpen = !isThumbOpen;
      while (digitalRead(pinThumb) == LOW) delay(1);
    }
    if (digitalRead(pinOther) == LOW) swCount3 += 1;//A3
    else swCount3 = 0;
    if (swCount3 == 10) {
      swCount3 = 0;
      isOtherLock = !isOtherLock;
      while (digitalRead(pinOther) == LOW) delay(1);
    }

    sensorValue = readSensor();
    sensorValue2 = readSensor2();

    delay(25);
    if (sensorValue < sensorMin) sensorValue = sensorMin;
    else if (sensorValue > sensorMax) sensorValue = sensorMax;
    if (sensorValue2 < sensorMin) sensorValue2 = sensorMin;
    else if (sensorValue2 > sensorMax) sensorValue2 = sensorMax;

    if (sensorValue > ((sensorMax - sensorMin) / 2 + sensorMin) )position = prePosition + speed;
    else if (sensorValue2 > ((sensorMax2 - sensorMin2) / 2 + sensorMin2) )position = prePosition - speed;
    else position = prePosition;
    if (position < positionMin) position = positionMin;
    if (position > positionMax) position = positionMax;
    prePosition = position;

    outIndex = map(position, positionMin, positionMax, outIndexOpen, outIndexClose);
    servoIndex.write(outIndex);
    if (!isOtherLock) {
      outOther = map(position, positionMin, positionMax, outOtherOpen, outOtherClose);
      servoOther.write(outOther);
    }
    if (isThumbOpen) servoThumb.write(outThumbOpen);
    else servoThumb.write(outThumbClose);
    if (onSerial) serialMonitor();
  }
}

//掌屈側センサの読み取り
int readSensor() {
  int i, sval;
  for (i = 0; i < 10; i++) {
    sval += analogRead(pinSensor);
  }
  sval = sval / 10;
  return sval;
}

//背屈側センサの読み取り
int readSensor2() {
  int i, sval;
  for (i = 0; i < 10; i++) {
    sval += analogRead(pinSensor2);
  }
  sval = sval / 10;
  return sval;
}

void calibration() {
  outIndex = outIndexOpen;
  servoIndex.write(outIndexOpen);
  servoOther.write(outOtherClose);
  servoThumb.write(outThumbOpen);
  position = positionMin;
  prePosition = positionMin;

  delay(200);
  if (onSerial) Serial.println("======calibration start======");

  sensorMax = readSensor();
  sensorMin = sensorMax - 50;
  sensorMax2 = readSensor2();
  sensorMin2 = sensorMax2 - 50;
  unsigned long time = millis();
  while ( millis() < time + 4000 ) {
    sensorValue = readSensor();
    sensorValue2 = readSensor2();
    delay(25);
    if ( sensorValue < sensorMin ) sensorMin = sensorValue;
    else if ( sensorValue > sensorMax )sensorMax = sensorValue;
    if ( sensorValue2 < sensorMin2 ) sensorMin2 = sensorValue2;
    else if ( sensorValue2 > sensorMax2 )sensorMax2 = sensorValue2;

    if (sensorValue > ((sensorMax - sensorMin) / 2 + sensorMin) )position = prePosition + speed;
    else if (sensorValue2 > ((sensorMax2 - sensorMin2) / 2 + sensorMin2) )position = prePosition - speed;
    else position = prePosition;
    if (position < positionMin) position = positionMin;
    if (position > positionMax) position = positionMax;
    prePosition = position;
    
    outIndex = map(position, positionMin, positionMax, outIndexOpen, outIndexClose);
    servoIndex.write(outIndex);

    if (onSerial) serialMonitor();
  }
  if (onSerial)  Serial.println("======calibration finish======");
}

void serialMonitor() {
  Serial.print("Min="); Serial.print(sensorMin);
  Serial.print(",Max="); Serial.print(sensorMax);
  Serial.print(",sensor="); Serial.print(sensorValue);
  Serial.print(",speed="); Serial.print(speed);
  Serial.print(",position="); Serial.print(position);
  Serial.print(",outIndex="); Serial.print(outIndex);
  Serial.print(",isThumbOpen="); Serial.print(isThumbOpen);
  Serial.print(",isOtherLock="); Serial.println(isOtherLock);
}

//アナログ入力をデジタル入力として使う関数　
boolean DigitalRead(const int pin) {
  if (analogRead(pin) > 512)return 1;
  else return 0;
}
