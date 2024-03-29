// Import necessary libraries for 7-segment display
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <ezButton.h>
#include "Adafruit_LEDBackpack.h"
#include <SoftwareSerial.h>

#define rxPin 0
#define txPin 1

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

//Assign pins
ezButton power(7);  // Power
ezButton play(10);  // Play/Pause
ezButton bounce(5); // Bounce Toggle
ezButton motion(9);   // Baby/Auto Mode
ezButton axis(11);  // Toggle 3 axis choices
ezButton timer(12); // Timer press in
#define speed_DT A5
#define speed_CLK A4
#define angle_DT A3
#define angle_CLK A2
#define time_DT A1
#define time_CLK A0
#define register_RCLK 4
#define register_SRCLK 6
#define register_SER 17 
#define ledPin 13
#define latchPin 4
#define clockPin 6
#define dataPin 8

// Initializing variables
int timer_state = 0;  //Crono = 1, stopwatch = 0
int outputAngleValue = 1;
int outputSpeedValue = 1;
int state = 0;
long message = 0;
/*
 * message bit meaning from left to right:
 * 1. Motion type
 * 2. Axis
 * 3. Angle
 * 4. Speed
 * 5. Bounce
 */
boolean powerState = 0;
boolean startState = 0;
int axisState = 3; //1 for pitch, 2 for roll, 3 for combine
boolean motionState = 0; //0 for auto mode, 1 for baby mode
int duration = 845;
boolean speedRotaryState = 0;
boolean angleRotaryState = 0;
boolean timeRotaryState = 0;
int bounceState = 0; //0 for off, 1 for on
boolean timerDir = 0; //0 for count down, 1 for count up
long timerReset = 0;
long targetTime = 0;
long displayLetterTime = 0;

uint16_t number[] = {0x0C3F,0x0406,0x00DB,0x008F,0x00E6,
                   /* 0      1      2       3       4*/
                     0x00ED,0x00FD,0x01401,0x00FF,0x00EF};
                   /* 5      6      7      8         9 */

int rotary(int CLK, int DT, boolean lastStateCLK){
  boolean currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      return 1;
    } else {
      // Encoder is rotating CW so increment
      return -1;
    }
    }else{
      return 0;
    }
}

byte smallLed(int speedValue, int angleValue, int axis){
  int ledRegister = 0;
  if (speedValue > 3){
    bitSet(ledRegister, 3);
  }
  if (speedValue > 6){
    bitSet(ledRegister, 2);
  }
  if (angleValue > 3){
    bitSet(ledRegister, 0);
  }
  if (angleValue > 6){
    bitSet(ledRegister, 1);
  }
  switch (axis){
    case 1:
    bitSet(ledRegister, 5);
    break;
    case 2:
    bitSet(ledRegister, 7);
    break;
    default:
    bitSet(ledRegister, 6);
    break;
  }
  return ledRegister;
}

void ledDisplay(int duration){
  if (duration > 5999){
    return;
  }
  int durationMin = duration / 60;
  int durationSec = duration - 60 * durationMin;
  int durationMin1 = durationMin / 10;
  durationMin = durationMin - durationMin1 * 10;
  int durationSec1 = durationSec /10;
  durationSec = durationSec - durationSec1 * 10;
  alpha4.writeDigitRaw(0, number[durationMin1]);
  alpha4.writeDigitRaw(1, number[durationMin]);
  alpha4.writeDigitRaw(2, number[durationSec1]);
  alpha4.writeDigitRaw(3, number[durationSec]);
  alpha4.writeDisplay();
}

void angleDisplay(int num){
  alpha4.writeDigitRaw(0, 0x00F7);
  alpha4.writeDigitRaw(1, 0x2136);
  alpha4.writeDigitRaw(2, 0x00BD);
  alpha4.writeDigitRaw(3, number[num]);
  alpha4.writeDisplay();
}

void speedDisplay(int num){
  alpha4.writeDigitRaw(0, 0x018D);
  alpha4.writeDigitRaw(1, 0x00F3);
  alpha4.writeDigitRaw(2, 0x120F);
  alpha4.writeDigitRaw(3, number[num]);
  alpha4.writeDisplay();
}

// Setup function
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  alpha4.begin(0x70);
  power.setDebounceTime(50); // set debounce time to 50 milliseconds
  play.setDebounceTime(50);
  bounce.setDebounceTime(50);
  motion.setDebounceTime(50);
  axis.setDebounceTime(50);
  timer.setDebounceTime(50);
  pinMode(angle_DT, INPUT);
  pinMode(angle_CLK, INPUT);
  pinMode(speed_DT, INPUT);
  pinMode(speed_CLK, INPUT);
  pinMode(time_DT, INPUT);
  pinMode(time_CLK, INPUT);
  pinMode(register_RCLK, OUTPUT);
  pinMode(register_SRCLK, OUTPUT);
  pinMode(register_SER, OUTPUT); 
  pinMode(ledPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

boolean lastStateAngleCLK = digitalRead(angle_CLK);
boolean lastStateSpeedCLK = digitalRead(speed_CLK);
boolean lastStateTimeCLK = digitalRead(time_CLK);

// Loop function
void loop() {
  power.loop(); // MUST call the loop() function first
  play.loop();
  bounce.loop();
  motion.loop();
  axis.loop();
  timer.loop();
  byte ledRegister = 0;
  
  powerState = !(power.getState());

  if (powerState){
    ledRegister = smallLed(outputSpeedValue, outputAngleValue, axisState);
    if (play.isReleased()){
      startState = !startState;
    }
    if (!startState){
      if (timer.isReleased()){
        timerDir = !timerDir;
        timerReset = 0;
      }
      if (!(timer.getState()) && timerReset == 0){
        timerReset = millis() + 3000;
      }else if (!(timer.getState()) && timerReset < millis()){
        duration = 0;
      }
      motionState = motion.getState();
      if (axis.isReleased()){
        if (axisState < 3){
          axisState++;
        }else{
          axisState = 1;
        }
        mySerial.write(30+axisState);
      }
      bounceState = bounce.getState();
    }
  }
  
  if (powerState){
    if (displayLetterTime < millis()){
      ledDisplay(duration);
    }
    state = 1;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
    digitalWrite(ledPin, HIGH);
    if (startState){
      mySerial.write(10+outputSpeedValue);
      mySerial.write(20+outputAngleValue);
      mySerial.write(40+bounceState);
      mySerial.write(100);
      if (timerDir){
        if (targetTime == 0){
          targetTime = millis() + 1000;
        }
        if (targetTime < millis()){
          duration++;
          targetTime = targetTime + 1000;
        }
      }else{
        if (targetTime == 0){           // Counting down
          targetTime = millis() + 1000;
        }else if (targetTime < millis()){
          duration--;
          targetTime = targetTime + 1000;
        }
      }
      state = 2;
      if (axisState == 1){
        state = 3;
      }else if (axisState == 2){
        state = 4;
      }
      if (motionState){
        state = 5;
      }
    }else{
      targetTime = 0;
      int angleDir = rotary(angle_CLK, angle_DT, lastStateAngleCLK);
      int speedDir = rotary(speed_CLK, speed_DT, lastStateSpeedCLK);
      int timeDir = rotary(time_CLK, time_DT, lastStateTimeCLK);
      lastStateAngleCLK = digitalRead(angle_CLK);
      lastStateSpeedCLK = digitalRead(speed_CLK);
      lastStateTimeCLK = digitalRead(time_CLK);
      if (angleDir == 1 && outputAngleValue < 9){
        outputAngleValue++;
        displayLetterTime = millis() + 2000;
        angleDisplay(outputAngleValue);
      }
      if (angleDir == -1 && outputAngleValue > 1){
        outputAngleValue--;
        displayLetterTime = millis() + 2000;
        angleDisplay(outputAngleValue);
      }
      if (speedDir == 1 && outputSpeedValue < 9){
        outputSpeedValue++;
        displayLetterTime = millis() + 2000;
        speedDisplay(outputSpeedValue);
      }
      if (speedDir == -1 && outputSpeedValue > 1){
        outputSpeedValue--;
        displayLetterTime = millis() + 2000;
        speedDisplay(outputSpeedValue);
      }
      if (timeDir == 1 && duration < 5939){
        duration += 60;
      }
      if (timeDir == -1 && duration > 30){
        duration -= 30;
      }
    }
  }else{
    alpha4.clear();
    alpha4.writeDisplay();
    targetTime = 0;
    outputSpeedValue = 1;
    outputAngleValue = 1;
    startState = 0;
    state = 1;
    duration = 845;
    digitalWrite(ledPin, LOW);
    ledRegister = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
  }
  if (!timerDir && duration < 0){
    startState = 0;
    duration = 845;
  }else if (timerDir && duration >= 5998){
    startState = 0;
  }

  switch (state){
    case 1:
    message = 0;
    break;
    case 2:
    message = 100;
    //message = 1000 + bounceState*100 + outputAngleValue * 10 + outputSpeedValue;
    break;
    case 3:
    message = 200;
    //message = 2000 + bounceState*100 + outputAngleValue * 10 + outputSpeedValue;
    break;
    case 4:
    message = 256;
    //message = 3000 + bounceState*100 + outputAngleValue * 10 + outputSpeedValue;
    break;
    case 5:
    message = 4;
    break;
  }
//  mySerial.write(message);
  }
