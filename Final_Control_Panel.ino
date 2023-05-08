// Import necessary libraries for 7-segment display
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

//Assign pins
#define powerPin 7
#define pausePin 10
#define bouncePin 5
#define motionPin 9
#define axisPin 11
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
#define latchPin 19
#define clockPin 18
#define dataPin 17

// Initializing variables
int outputAngleValue = 0;
int outputSpeedValue = 0;
int state = 0;
long message = 0;
/*
 * message bit meaning from left to right:
 * 1. On/Off
 * 2.Speed
 * 3.Angle
 * 4. axis
 * 5. Motion type
 */
boolean power = 0;
boolean start = 0;
int axis = 0; //0 for pitch, 1 for roll, 2 for combine
boolean motion = 0; //0 for auto mode, 1 for baby mode
long duration = 845000;
boolean speedRotaryState = 0;
boolean angleRotaryState = 0;
boolean timeRotaryState = 0;

boolean button(int pinNumber){
  byte mask = 0;
  for (int i = 1; i <= 4; i++){
    mask = mask << 1;
    mask = mask + digitalRead(pinNumber);
  }
  return (mask == 15);
}

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

byte smallLed(int speedValue, int angleValue, int motion){
  int ledRegister = 0;
  if (speedValue > 1){
    bitSet(ledRegister, 2);
  }else if (speedValue == 3){
    bitSet(ledRegister, 3);
  }
  if (angleValue > 1){
    bitSet(ledRegister, 4);
  }else if (speedValue == 3){
    bitSet(ledRegister, 5);
  }
  switch (motion){
    case 1:
    bitSet(ledRegister, 6);
    break;
    case 2:
    bitSet(ledRegister, 7);
    break;
    default:
    bitSet(ledRegister, 8);
    break;
  }
  return ledRegister;
}

// Setup function
void setup() {
  Serial.begin(9600);
  pinMode(powerPin, INPUT_PULLUP);
  pinMode(pausePin, INPUT_PULLUP);
  pinMode(bouncePin, INPUT_PULLUP);
  pinMode(motionPin, INPUT_PULLUP);
  pinMode(axisPin, INPUT_PULLUP);
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
  byte ledRegister = 0;
  
  power = digitalRead(powerPin);

  if (power){
    ledRegister = smallLed(outputSpeedValue, outputAngleValue, motion);
    bitSet(ledRegister, 1);
    if (button(pausePin)){
      start = !start;
    }
    if (start == 0){
      if (button(motionPin)){
        motion = !motion;
      }
      if (button(axisPin)){
        if (axis < 3){
          axis++;
        }else{
          axis = 0;
        }
      }
    }
  }
  
  if (power){
    state = 1;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
    digitalWrite(ledPin, HIGH);
    if (start){
      state = 2;
      if (axis == 1){
        state = 3;
      }else if (axis == 2){
        state = 4;
      }
      if (motion){
        state = 5;
      }
    }else{
      if (rotary(angle_CLK,angle_DT,lastStateAngleCLK) == 1){
        if (outputAngleValue < 3){
          outputAngleValue++;
          lastStateAngleCLK = !lastStateAngleCLK;
        }
      }
      if (rotary(angle_CLK,angle_DT,lastStateAngleCLK) == -1){
        if (outputAngleValue >1){
          outputAngleValue--;
          lastStateAngleCLK = !lastStateAngleCLK;
        }
      }
      if (rotary(speed_CLK,speed_DT,lastStateSpeedCLK) == 1){
        if (outputSpeedValue < 3){
          outputSpeedValue++;
          lastStateSpeedCLK = !lastStateSpeedCLK;
        }
      }
      if (rotary(speed_CLK,speed_DT,lastStateSpeedCLK) == -1){
        if (outputSpeedValue < 3){
          outputSpeedValue--;
          lastStateSpeedCLK = !lastStateSpeedCLK;
        }
      }
    }
  }else{
    outputSpeedValue = 1;
    outputAngleValue = 1;
    start = 0;
    state = 1;
    duration = 845000;
  }

  switch (state){
    case 1:
    message = 0;
    break;
    case 2:
    message = 1 + outputSpeedValue * 10 + outputAngleValue * 100;
    break;
    case 3:
    message = 1 + outputSpeedValue * 10 + outputAngleValue * 100 + 1000;
    break;
    case 4:
    message = 1 + outputSpeedValue * 10 + outputAngleValue * 100 + 2000;
    break;
    case 5:
    message = 1 + 10000;
    break;
  }
  Serial.println(message);

  /*
  if (button(enablePin)) {
      matrix.drawColon(true);
      matrix.writeDisplay();
      // Check angle pins
      Serial.print("Angle:");
      Serial.println(outputAngleValue);
      Serial.print("Speed:");
      Serial.println(outputSpeedValue);
      delay(1000);
      
      if (!button(speed1Pin)) {
        if (button(speed2Pin)){
          outputSpeedValue = 1;
        }else{
          outputSpeedValue  = 2;
        }
      } else{
        outputSpeedValue = 3;
      }
      // Check speed pins
            if (!button(angle1Pin)) {
        if (button(angle2Pin)){
          outputAngleValue = 1;
        }else{
          outputAngleValue  = 2;
        }
      } else{
        outputAngleValue = 3;
      }
      int durationMin = duration / 60000;
      int ledOutput = durationMin*100 + (duration / 1000) - durationMin * 60;
      ledDisplay(ledOutput);
      // Check start pin
      if (button(startPin)) {
        delay(1000);
        // Check if time is larger than 0
        if (duration > 0) {
          // Check if auto pin is 1
            long previousTime = millis();
            while (button(enablePin) && !button(startPin) && duration > 0){
              matrix.drawColon(true);
              matrix.writeDisplay();
              long currentTime = millis();
              Serial.println("on");
              delay(1000);
              duration -= currentTime - previousTime;
              previousTime = currentTime;
              int durationMin = duration / 60000;
              int ledOutput = durationMin*100 + (duration / 1000) - durationMin * 60;
              ledDisplay(ledOutput);
            }
            
          } else {
            int previousTime = 1/1000 * millis();
            while (button(enablePin) && !button(startPin) && duration > 0){
              Serial.println("Gamebar");
              int currentTime = 1/1000 * millis();
              duration -= currentTime - previousTime;
              previousTime = currentTime;
            }
          }
          delay(1250);               
        }

      }else{
        matrix.print("");
        matrix.writeDisplay();
      }
      */
  }
