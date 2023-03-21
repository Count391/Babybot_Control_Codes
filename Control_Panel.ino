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
long duration = 845000;

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
      matrix.drawColon(true);
      matrix.writeDisplay();
      // Check angle pins
      if (!button(angle1Pin)) {
        if (button(angle2Pin)){
          outputAngleValue = 1;
        }else{
          outputAngleValue  = 2;
        }
      } else{
        outputAngleValue = 3;
      }
      // Check speed pins
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
  }
