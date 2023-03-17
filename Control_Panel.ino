// Assigning pins
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

int enablePin = 7;
int startPin = 8;
int timeUpPin = 9;
int timeDownPin = 10;
int angle1Pin = 11;
int angle2Pin = 12;
int angle3Pin = 13;
int speed1Pin = A0;
int speed2Pin = A1;
int speed3Pin = A2;
int autoPin = A3;

// Initializing variables
int outputAngleValue = 1;
int outputSpeedValue = 1;
int duration = 0;

boolean button(int pinNumber){
  return digitalRead(pinNumber);
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
  pinMode(angle3Pin, INPUT_PULLUP);
  pinMode(speed1Pin, INPUT_PULLUP);
  pinMode(speed2Pin, INPUT_PULLUP);
  pinMode(speed3Pin, INPUT_PULLUP);
  pinMode(autoPin, INPUT_PULLUP);
}

// Loop function
void loop() {
  // Check if enable pin is 1
  if (button(enablePin)) {
    matrix.drawColon(true);
    // Check angle pins
    if (button(angle1Pin)) {
      outputAngleValue = 1;
    } else if (button(angle2Pin)) {
      outputAngleValue = 2;
    } else if (button(angle3Pin)) {
      outputAngleValue = 3;
    }
    // Check speed pins
    if (button(speed1Pin)) {
      outputSpeedValue = 1;
    } else if (button(speed2Pin)) {
      outputSpeedValue = 2;
    } else if (button(speed3Pin)) {
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
      // Check if time is larger than 0
      if (duration > 0) {
        // Check if auto pin is 1
        if (button(autoPin)) {
          int previousTime = 1/1000 * millis();
          while (button(enablePin) && button(startPin) && duration > 0){
            Serial.println("Output");
            int currentTime = 1/1000 * millis();
            duration -= currentTime - previousTime;
            previousTime = currentTime;
          }
          
        } else {
          int previousTime = 1/1000 * millis();
          while (button(enablePin) && button(startPin) && duration > 0){
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
