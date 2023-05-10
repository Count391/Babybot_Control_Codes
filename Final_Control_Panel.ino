// Import necessary libraries for 7-segment display
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <ezButton.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

//Assign pins
ezButton power(7);  // Power
ezButton play(10);  // Play/Pause
ezButton bounce(5); // Bounce Toggle
ezButton motion(9);   // Baby/Auto Mode
ezButton axis(11);  // Toggle 3 axis choices
ezButton timer(12); // Timer press in
#define angle_DT A5
#define angle_CLK A4
#define speed_DT A3
#define speed_CLK A2
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
int axisState = 0; //0 for pitch, 1 for roll, 2 for combine
boolean motionState = 0; //0 for auto mode, 1 for baby mode
long duration = 845000;
boolean speedRotaryState = 0;
boolean angleRotaryState = 0;
boolean timeRotaryState = 0;
int bounceState = 0; //0 for off, 1 for on
boolean timerDir = 0; //0 for count down, 1 for count up
int durationMin = 0;
int durationSec = 0;
int timerReset = 0;
int targetTime = 0;

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
  if (speedValue > 1){
    bitSet(ledRegister, 4);
  }
  if (speedValue == 3){
    bitSet(ledRegister, 3);
  }
  if (angleValue > 1){
    bitSet(ledRegister, 2);
  }
  if (angleValue == 3){
    bitSet(ledRegister, 1);
  }
  switch (axis){
    case 1:
    bitSet(ledRegister, 5);
    break;
    case 2:
    bitSet(ledRegister, 6);
    break;
    default:
    bitSet(ledRegister, 7);
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

// Setup function
void setup() {
  Serial.begin(9600);
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
    bitSet(ledRegister, 0);
    if (play.isReleased()){
      startState = !startState;
    }
    if (!startState){
      if (timer.isReleased()){
        timerDir = !timerDir;
        timerReset = 0;
      }
      if (timer.isPressed() && timerReset == 0){
        timerReset = millis() + 3000;
      }else if (timer.isPressed() && timerReset < millis()){
        duration = 0;
      }
      motionState = motion.getState();
      if (axis.isReleased()){
        if (axisState < 2){
          axisState++;
        }else{
          axisState = 0;
        }
      }
      bounceState = bounce.getState();
    }
  }
  
  if (powerState){
    ledDisplay(duration);
    state = 1;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
    digitalWrite(ledPin, HIGH);
    if (startState){
      if (timerDir){
        if (targetTime == 0){
          targetTime = millis() + 1000;
        }else if (targetTime < millis()){
          duration++;
          targetTime = targetTime + 1000;
        }
      }else{
        if (targetTime = 0){
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
      if (angleDir == 1 && outputAngleValue < 3){
        outputAngleValue++;
      }
      if (angleDir == -1 && outputAngleValue > 1){
        outputAngleValue--;
      }
      if (speedDir == 1 && outputSpeedValue < 3){
        outputSpeedValue++;
      }
      if (speedDir == -1 && outputSpeedValue > 1){
        outputSpeedValue--;
      }
      if (timeDir == 1 && duration < 5999){
        duration += 60;
      }
      if (timeDir == -1 && duration > 0){
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
    duration = 845000;
    digitalWrite(ledPin, LOW);
    ledRegister = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
  }

  switch (state){
    case 1:
    message = 0;
    break;
    case 2:
    message = bounceState + outputAngleValue * 100 + outputSpeedValue * 10;
    break;
    case 3:
    message = bounceState + outputAngleValue * 100 + outputSpeedValue * 10 + 1000;
    break;
    case 4:
    message = bounceState + outputAngleValue * 100 + outputSpeedValue * 10 + 2000;
    break;
    case 5:
    message = 10000;
    break;
  }
  Serial.println(message);
  }
