// Import necessary libraries for 7-segment display
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();


// Import library for button presses
#include <ezButton.h>

// Assign Pins for Buttons/Switches
ezButton power(7);  // Power
ezButton play(10);  // Play/Pause
ezButton bounce(5); // Bounce Toggle
ezButton mode(9);   // Baby/Auto Mode
ezButton axis(11);  // Toggle 3 axis choices
ezButton timer(12); // Timer press in

// Assign Pins for Encoders
#define angle_DT A5
#define angle_CLK A4
#define speed_DT A3
#define speed_CLK A2
#define time_DT A1
#define time_CLK A0

// Assign Pins for LEDs
#define register_RCLK 4
#define register_SRCLK 6
#define register_SER 8
#define ledPin 13

// Assign Pins for rx/tx
#define rxPin 0
#define txPin 1

// Assign Pins for Screen Backpack
#define bp_CLK 3
#define bp_DT 2

// Initializing Variables
int play_state = 0;   // Play = 1, Pause = 0
int axis_state = 1;   // Combined = 1, Pitch = 2, Roll = 3
int timer_state = 0;  // Crono = 1, Stopwatch = 0

int angle_value = 1;
int speed_value = 1;
int angle_direction = 0;
int speed_direction = 0;
int time_direction = 0;
long duration = 15;   // 15 minutes
int lastStateAngleCLK;
int lastStateSpeedCLK;
int lastStateTimeCLK;

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

void setup() {
  // Start Serial Monitor
  Serial.begin(9600);

  // Define debounce time for buttons/switches
  power.setDebounceTime(50); // set debounce time to 50 milliseconds
  play.setDebounceTime(50);
  bounce.setDebounceTime(50);
  mode.setDebounceTime(50);
  axis.setDebounceTime(50);
  timer.setDebounceTime(50);
  
  // Define pin modes of rotary encoders
  pinMode(angle_DT, INPUT);
  pinMode(angle_CLK, INPUT);
  pinMode(speed_DT, INPUT);
  pinMode(speed_CLK, INPUT);
  pinMode(time_DT, INPUT);
  pinMode(time_CLK, INPUT);

  // Define pin modes of LEDs
  pinMode(register_RCLK, OUTPUT);
  pinMode(register_SRCLK, OUTPUT);
  pinMode(register_SER, OUTPUT); 
  pinMode(ledPin, OUTPUT);

  // Initialize last state for rotaries
  lastStateAngleCLK = digitalRead(angle_CLK);
  lastStateSpeedCLK = digitalRead(speed_CLK);
  lastStateTimeCLK = digitalRead(time_CLK);
}


void loop() {
  power.loop(); // MUST call the loop() function first
  play.loop();
  bounce.loop();
  mode.loop();
  axis.loop();
  timer.loop();
  angle_direction = rotary(angle_CLK, angle_DT, lastStateAngleCLK);
  speed_direction = rotary(speed_CLK, speed_DT, lastStateSpeedCLK);
  time_direction = rotary(time_CLK, time_DT, lastStateTimeCLK);
  lastStateAngleCLK = digitalRead(angle_CLK);
  lastStateSpeedCLK = digitalRead(speed_CLK);
  lastStateTimeCLK = digitalRead(time_CLK);

  if (power.isPressed())
    Serial.println("The power button: OFF -> ON");  // Update power state

  if (power.isReleased())
    Serial.println("The power button: ON -> OFF");

  if (bounce.isPressed())
    Serial.println("The bounce switch: OFF -> ON");  // Upddate bounce state

  if (bounce.isReleased())
    Serial.println("The bounce switch: ON -> OFF");

  if (mode.isPressed())
    Serial.println("Baby Mode");                    // Update mode state

  if (mode.isReleased())
    Serial.println("Auto Mode");
  
  if (play.isReleased()){                          // Start the BabyBot in accordance to all states
    if (play_state == 0){
      Serial.println("Start BabyBot");
      play_state = 1;
    }
    else{
      Serial.println("Stop  BabyBot");
      play_state = 0;
    }
  }

  if (timer.isReleased()){                          // Toggle timer between chrono and stopwatch
    if (timer_state == 0){
      Serial.println("Counting Up");
      duration = 0;
      timer_state = 1;
    }
    else{
      Serial.println("Counting Down");
      duration = 15;
      timer_state = 0;
    }
  }
  
  if (axis.isReleased()){                           // Update axis state
    if (axis_state == 1){
      Serial.println("Combined Axis Motion");
      axis_state = 2;
    }
    else  if (axis_state == 2){
      Serial.println("Pitch Axis Only");
      axis_state = 3;
    }
    else{
      Serial.println("Roll Axis Only");
      axis_state = 1;
    }
  }

  if (angle_direction == -1){       // Based on direction of rotary, change angle value [1,3]
    if(angle_value == 1)
      Serial.println("Angle Level: 1");
      //continue;
    else {
      angle_value--;
      Serial.print("Angle Level: ");
      Serial.println(angle_value);
    }
  }
  else if (angle_direction == 1){
    if(angle_value == 3)
      Serial.println("Angle Level: 3");
      //continue;
    else {
      angle_value++;
      Serial.print("Angle Level: ");
      Serial.println(angle_value);
    }
  }

  if (speed_direction == -1){       // Based on direction of rotary, change speed value [1,3]
    if(speed_value == 1)
      Serial.println("Speed Value: 1");
      //continue;
    else {
      speed_value--;
      Serial.print("Speed Level: ");
      Serial.println(speed_value);
    }
  }
  else if (speed_direction == 1){
    if(speed_value == 3)
      Serial.println("Speed Value: 3");
      //continue;
    else {
      speed_value++;
      Serial.print("Speed Level: ");
      Serial.println(speed_value);
    }
  }

  if(timer_state == 0){
    if (time_direction == -1){       // Based on direction of rotary, change timer value
      if(duration == 5)
        Serial.println("Timer at 5:00");
        //continue;
      else {
        duration = duration - 5;
        Serial.print("Timer at ");
        Serial.print(duration);
        Serial.println(":00");
      }
    }
    else if (time_direction == 1){
      duration = duration + 5;
      Serial.print("Timer at ");
      Serial.print(duration);
      Serial.println(":00");
    }
  }
}
