// Import necessary libraries for 7-segment display
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

//Assign pins
int enablePin = 7;
int startPin = 8;
int timeUpPin = 9;
int timeDownPin = 10;
int angle1Pin = 11;
int angle2Pin = 12;
int speed1Pin = A0;
int speed2Pin = A1;
int autoPin = A3;

// Initializing variables
int outputAngleValue = 1;
int outputSpeedValue = 1;
int duration = 1000;

boolean button(int pinNumber){
  return !digitalRead(pinNumber);
}

void ledDisplay(int ledOutput){
  matrix.println(ledOutput, DEC);
  matrix.writeDisplay();
}

// Setup function
void setup() {
  Serial.begin(9600);
  matrix.begin(0x70);
  pinMode(enablePin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(timeUpPin, INPUT_PULLUP);
  pinMode(timeDownPin, INPUT_PULLUP);
  pinMode(angle1Pin, INPUT_PULLUP);
  pinMode(angle2Pin, INPUT_PULLUP);
  pinMode(speed1Pin, INPUT_PULLUP);
  pinMode(speed2Pin, INPUT_PULLUP);
  pinMode(autoPin, INPUT_PULLUP);
}

// Loop function
void loop() {
  // Check if enable pin is 1
  if (button(enablePin)) {
    delay(2000);
    while (!button(enablePin){
      matrix.drawColon(true);
      // Check angle pins
      if (button(!angle1Pin)) {
        if (button(angle2Pin){
          outputAngleValue = 1;
        }else{
          outputAngleValue  = 2;
        }
      } else{
        outputAngleValue = 3;
      }
      // Check speed pins
      if (button(!speed1Pin)) {
        if (button(speed2Pin){
          outputSpeedValue = 1;
        }else{
          outputSpeedValue  = 2;
        }
      } else{
        outputSpeedValue = 3;
      }
      // Check timeUp pin
      if (button(timeUpPin)) {
        if (duration < 600) {
          duration += 60;
          delay(750);
        }
      }
      // Check timeDown pin
      if (button(timeDownPin)) {
        if (duration > 0) {
          duration -= 60;
          delay(750);
        }
      }
      int ledOutput = duration / 60 * 100 + duration % 60;
      ledDisplay(ledOutput);
      // Check start pin
      if (button(startPin)) {
        delay(2000);
        while (!button(startPin){
        // Check if time is larger than 0
        if (duration > 0) {
          // Check if auto pin is 1
            int previousTime = 1/1000 * millis();
            while (!button(enablePin) && button(startPin) && duration > 0){
              Serial.println("Output");
              int currentTime = 1/1000 * millis();
              duration -= currentTime - previousTime;
              previousTime = currentTime;
            }
            
          } else {
            int previousTime = 1/1000 * millis();
            while (!button(enablePin) && button(startPin) && duration > 0){
              Serial.println("Gamebar");
              int currentTime = 1/1000 * millis();
              duration -= currentTime - previousTime;
              previousTime = currentTime;
            }
          }
          int ledOutput = duration / 60 * 100 + duration % 60;
          ledDisplay(ledOutput);
        }
      }
    }
  }
}
