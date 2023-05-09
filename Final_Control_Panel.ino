// Import necessary libraries for 7-segment display
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <ezButton.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

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
#define com 1
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
    bitSet(ledRegister, 1);
  }
  if (speedValue == 3){
    bitSet(ledRegister, 2);
  }
  if (angleValue > 1){
    bitSet(ledRegister, 3);
  }
  if (angleValue == 3){
    bitSet(ledRegister, 4);
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

// Setup function
void setup() {
  Serial.begin(9600);
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
    state = 1;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
    digitalWrite(ledPin, HIGH);
    if (startState){
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
      int angle_direction = rotary(angle_CLK, angle_DT, lastStateAngleCLK);
      int speed_direction = rotary(speed_CLK, speed_DT, lastStateSpeedCLK);
      lastStateAngleCLK = digitalRead(angle_CLK);
      lastStateSpeedCLK = digitalRead(speed_CLK);
      if (angle_direction == 1 && outputAngleValue < 3){
        outputAngleValue++;
      }
      if (angle_direction == -1 && outputAngleValue >1){
        outputAngleValue--;
      }
      if (speed_direction == 1 && outputSpeedValue < 3){
        outputSpeedValue++;
      }
      if (speed_direction == -1 && outputSpeedValue >1){
        outputSpeedValue--;
      }
    }
  }else{
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
