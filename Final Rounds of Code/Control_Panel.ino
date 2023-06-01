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
ezButton power(10);  // Power
ezButton play(11);  // Play/Pause
ezButton bounce(5); // Bounce Toggle
ezButton motion(7);   // Baby/Auto Mode
ezButton axis(9);  // Toggle 3 axis choices
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
boolean powerInd = 1;
long durationDisplayTime = 0; 
int darkTime = 0;
int timer_state = 0;  //Crono = 1, stopwatch = 0
int outputAngleValue = 1;
int outputSpeedValue = 1;
boolean powerState = 0;
boolean startState = 0;
int axisState = 0; //0 for combined, 1 for roll, 2 for pitch
boolean motionState = 0; //0 for auto mode, 1 for baby mode
int duration = 900;
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
                   
                   /* A/0    B/1    C/2    D/3    E/4    F/5    G/6 */
uint16_t letter[] = {0x00F7,0x128F,0x0039,0x120F,0x00F9,0x00F1,0x00BD,
                   /* H/7    I/8    J/9    K/10  L/11   M/12   N/13 */
                     0x00F6,0x1209,0x001E,0x2470,0x0038,0x0536,0x2136,
                   /* O/14  P/15   Q/16   R/17   S/18   T/19   U/20 */
                     0x003F,0x00F3,0x203F,0x20F3,0x018D,0x1201,0x003E,
                   /* V/21  W/22   X/23   Y/24   Z/25 */
                     0x0C30,0x2836,0x2D00,0x1500,0x0C09};

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
  bitSet(ledRegister, 4);
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
    alpha4.writeDigitRaw(0, letter[0]);
    alpha4.writeDigitRaw(1, letter[13]);
    alpha4.writeDigitRaw(2, 0x0000);
    alpha4.writeDigitRaw(3, number[angleValue]);
    alpha4.writeDisplay();
  }
  if (speedDir != 0){
    alpha4.writeDigitRaw(0, letter[18]);
    alpha4.writeDigitRaw(1, letter[15]);
    alpha4.writeDigitRaw(2, 0x0000);
    alpha4.writeDigitRaw(3, number[speedValue]);
    alpha4.writeDisplay();
  }
  mySerial.write(30 + speedValue);
  mySerial.write(20 + angleValue);
}

void welcome(){
    alpha4.writeDigitRaw(1, letter[1]);
    alpha4.writeDigitRaw(2, letter[1]);
    alpha4.writeDisplay();
    delay(1500);
    alpha4.writeDigitRaw(0, letter[22]);
    alpha4.writeDigitRaw(1, letter[4]);
    alpha4.writeDigitRaw(2, letter[11]);
    alpha4.writeDigitRaw(3, letter[2]);
    alpha4.writeDisplay();
    delay(700);
    alpha4.writeDigitRaw(0, letter[4]);
    alpha4.writeDigitRaw(1, letter[11]);
    alpha4.writeDigitRaw(2, letter[2]);
    alpha4.writeDigitRaw(3, letter[14]);
    alpha4.writeDisplay();
    delay(700);
    alpha4.writeDigitRaw(0, letter[11]);
    alpha4.writeDigitRaw(1, letter[2]);
    alpha4.writeDigitRaw(2, letter[14]);
    alpha4.writeDigitRaw(3, letter[12]);
    alpha4.writeDisplay();
    delay(700);
    alpha4.writeDigitRaw(0, letter[2]);
    alpha4.writeDigitRaw(1, letter[14]);
    alpha4.writeDigitRaw(2, letter[12]);
    alpha4.writeDigitRaw(3, letter[4]);
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

void emptyLed(){
  alpha4.clear();
  alpha4.writeDisplay();
}

// Setup function
void setup() {
  mySerial.begin(9600);
  delay(2000);
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

  if (powerInd && powerState){
    welcome();
    powerInd = !powerInd;
  }

  if (powerState){
    ledRegister = smallLed(outputSpeedValue, outputAngleValue, axisState);
    digitalWrite(latchPin, LOW);                          //Use shift register to light up small LEDs
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
    digitalWrite(ledPin, HIGH);
    if (play.isReleased()){                               //Check pause state
      if (startState){
        mySerial.write(51);
      }else{
        mySerial.write(52);
      }
      startState = !startState;
    }
    if (!startState){
      if (motion.isPressed() || motion.isReleased()){
        motionState = motion.getState();                    //Check for button pushing
        mySerial.write(40 + motionState);
      }
      if (bounce.isReleased() || bounce.isPressed()){
        mySerial.write(53);
      }
      if (axis.isReleased()){
        if (axisState < 2){
          axisState++;
        }else{
          axisState = 0;
        }
        mySerial.write(10 + axisState);
      }
      int angleDir = rotary(angle_CLK, angle_DT, lastStateAngleCLK); //Check rotary encoder inputs
      int speedDir = rotary(speed_CLK, speed_DT, lastStateSpeedCLK);
      int timeDir = rotary(time_CLK, time_DT, lastStateTimeCLK);
      lastStateAngleCLK = digitalRead(angle_CLK);
      lastStateSpeedCLK = digitalRead(speed_CLK);
      lastStateTimeCLK = digitalRead(time_CLK);
      outputAngleValue = outputAngleValue + angleDir;     //Change the output values
      outputSpeedValue = outputSpeedValue + speedDir;
      if (timeDir != 0){
        duration = (duration / 60 + timeDir) * 60;
      }
      outputAngleValue = AngSpdBoundaryCheck(outputAngleValue);
      outputSpeedValue = AngSpdBoundaryCheck(outputSpeedValue);
      duration = durationBoundaryCheck(duration);
      if (angleDir != 0 || speedDir != 0){                //Display changes
        displayLetterTime = millis() + 3000;
        letterDisplay(angleDir, speedDir, outputAngleValue, outputSpeedValue);
      }
      if (displayLetterTime < millis()){                  //Display time if no recent change in angle and speed
        if (durationDisplayTime > millis()){
          ledDisplay(duration);
        }else{
          emptyLed();
          darkTime = millis() - durationDisplayTime;
          if (darkTime > 200){
            durationDisplayTime = millis() + 1000;
          }
        }
      }
      if (timer.isReleased()){                            //Flip timer direction once timer knob is pushed
        if (timerDir == -1){
          timerDir = 1;
          duration = 0;
        }else{
          timerDir = -1;
        }
      }
      if (!(timer.getState())){        //Reset timer to 0 when hold for 2.5s
        timerReset = millis() + 2500;
      }else if (!(timer.getState()) && timerReset < millis()){
        duration = 0;
      }
    }else{                                                //When started, time flows
      if (targetTime < millis()){
        targetTime = millis() + 1000;
        duration = duration + timerDir;
        ledDisplay(duration);
      }
    }
  }else{
    mySerial.write(50);
    powerInd = 1;
    alpha4.clear();
    alpha4.writeDisplay();
    targetTime = 0;
    outputSpeedValue = 1;
    outputAngleValue = 1;
    startState = 0;
    duration = 900;
    digitalWrite(ledPin, LOW);
    ledRegister = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, ledRegister);
    digitalWrite(latchPin, HIGH);
  }
  if (timerDir == -1 && duration < 0){
    mySerial.write(52);
    startState = 0;
    duration = 900;
  }else if (timerDir == 1 && duration >= 5998){
    startState = 0;
    mySerial.write(52);
  }
}
