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
//Assign Pin numbers
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
boolean powerState = 0;
boolean startState = 0;
int axisState = 0; //0 for combined, 1 for roll, 2 for pitch
boolean motionState = 0; //0 for auto mode, 1 for baby mode
int duration = 845;
boolean speedRotaryState = 0;
boolean angleRotaryState = 0;
boolean timeRotaryState = 0;
int bounceState = 0; //0 for off, 1 for on
int timerDir = -1; //-1 for count down, 1 for count up
long timerReset = 0;
long targetTime = 0;
long displayLetterTime = 0;

uint16_t number[] = {0x0C3F,0x0406,0x00DB,0x008F,0x00E6,
                   /* 0      1      2       3       4*/
                     0x00ED,0x00FD,0x01401,0x00FF,0x00EF};
                   /* 5      6      7      8         9 */
uint16_t letter[] = {0x00F7, 0x120F, 0x00BD, 0x2136, 0x00F3, 0x1201, 
                   /*  A       D        G       N       P       S*/
                   0x00F6, 0x1209, 0x128F, 0x1500, 0x003F, 0x1201};
                   /*H       I        B       Y      O       T*/

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

void letterDisplay(int angleDir, int speedDir, int angleValue, int speedValue){
  if (angleDir != 0){
    alpha4.writeDigitRaw(0, letter[1]);
    alpha4.writeDigitRaw(1, letter[4]);
    alpha4.writeDigitRaw(2, letter[3]);
    alpha4.writeDigitRaw(3, number[angleValue]);
    alpha4.writeDisplay();
  }
  if (speedDir != 0){
    alpha4.writeDigitRaw(0, letter[6]);
    alpha4.writeDigitRaw(1, letter[5]);
    alpha4.writeDigitRaw(2, letter[2]);
    alpha4.writeDigitRaw(3, number[speedValue]);
    alpha4.writeDisplay();
  }
  Serial.write(30 + speedValue);
  Serial.write(20 + angleValue);
}

void welcome(){
    alpha4.writeDigitRaw(1, letter[7]);
    alpha4.writeDigitRaw(2, letter[8]);
    alpha4.writeDisplay();
    delay(1500);
    alpha4.writeDigitRaw(0, letter[9]);
    alpha4.writeDigitRaw(1, letter[1]);
    alpha4.writeDigitRaw(2, letter[9]);
    alpha4.writeDigitRaw(3, letter[10]);
    alpha4.writeDisplay();
    delay(700);
    alpha4.writeDigitRaw(0, letter[1]);
    alpha4.writeDigitRaw(1, letter[9]);
    alpha4.writeDigitRaw(2, letter[10]);
    alpha4.writeDigitRaw(3, letter[9]);
    alpha4.writeDisplay();
    delay(700);
    alpha4.writeDigitRaw(0, letter[9]);
    alpha4.writeDigitRaw(1, letter[10]);
    alpha4.writeDigitRaw(2, letter[9]);
    alpha4.writeDigitRaw(3, letter[11]);
    alpha4.writeDisplay();
    delay(700);
    alpha4.writeDigitRaw(0, letter[10]);
    alpha4.writeDigitRaw(1, letter[9]);
    alpha4.writeDigitRaw(2, letter[11]);
    alpha4.writeDigitRaw(3, letter[12]);
    alpha4.writeDisplay();
    delay(1000);
}

int AngSpdBoundaryCheck(int num){
  if (num == 0){
    return 1;
  }
  if (num == 10){
    return 9;
  }
  return num;
}

long durationBoundaryCheck(long duration){
  if (duration < 0){
    return 0;
  }
  if (duration > 5999){
    return 5999;
  }
  return duration;
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
  
  powerState = !(power.getState());                       //Check power state

  if (power.isReleased()){
    welcome();
  }

  if (powerState){
    ledRegister = smallLed(outputSpeedValue, outputAngleValue, axisState);
    digitalWrite(latchPin, LOW);                          //Use shift register to light up small LEDs
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
    digitalWrite(ledPin, HIGH);
    if (play.isReleased()){                               //Check pause state
      if (startState){
        if (bounceState){
          Serial.write(53);
        }else{
          Serial.write(51);
        }
      }else{
        Serial.write(52);
      }
      startState = !startState;
    }
    if (!startState){
      if (motion.isPressed() || motion.isReleased()){
        motionState = motion.getState();                    //Check for button pushing
        Serial.write(40 + motionState);
      }
      bounceState = bounce.getState();
      if (axis.isReleased()){
        if (axisState < 2){
          axisState++;
        }else{
          axisState = 0;
        }
        Serial.write(10 + axisState);
      }
      int angleDir = rotary(angle_CLK, angle_DT, lastStateAngleCLK); //Check rotary encoder inputs
      int speedDir = rotary(speed_CLK, speed_DT, lastStateSpeedCLK);
      int timeDir = rotary(time_CLK, time_DT, lastStateTimeCLK);
      lastStateAngleCLK = digitalRead(angle_CLK);
      lastStateSpeedCLK = digitalRead(speed_CLK);
      lastStateTimeCLK = digitalRead(time_CLK);
      outputAngleValue = outputAngleValue + angleDir;     //Change the output values
      outputSpeedValue = outputSpeedValue + speedDir;
      duration = outputAngleValue + timeDir * 30;
      outputAngleValue = AngSpdBoundaryCheck(outputAngleValue);
      outputSpeedValue = AngSpdBoundaryCheck(outputSpeedValue);
      duration = durationBoundaryCheck(duration);
      if (angleDir != 0 || speedDir != 0){                //Display changes
        displayLetterTime = millis() + 3000;
        letterDisplay(angleDir, speedDir, outputAngleValue, outputSpeedValue);
      }
      if (displayLetterTime < millis()){                  //Display time if no recent change in angle and speed
        ledDisplay(duration);
      }
      if (timer.isReleased()){                            //Flip timer direction once timer knob is pushed
        timerDir = (-1) * timerDir;
        timerReset = 0;
      }
      if (!(timer.getState()) && timerReset == 0){        //Reset timer to 0 when hold for 2.5s
        timerReset = millis() + 2500;
      }else if (!(timer.getState()) && timerReset < millis()){
        duration = 0;
      }
    }else{                                                //When started, time flows
      Serial.write(52);
      if (targetTime < millis()){
        targetTime = millis() + 1000;
        duration = duration + timerDir;
      }
    }
  }else{
    Serial.write(50);
    alpha4.clear();
    alpha4.writeDisplay();
    targetTime = 0;
    outputSpeedValue = 1;
    outputAngleValue = 1;
    startState = 0;
    duration = 845;
    digitalWrite(ledPin, LOW);
    ledRegister = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
  }
  if (timerDir == -1 && duration < 0){
    startState = 0;
    duration = 845;
  }else if (timerDir == 1 && duration >= 5998){
    startState = 0;
  }
}
