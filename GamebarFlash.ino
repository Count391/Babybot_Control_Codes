#include <ezButton.h>

#define ledPin1 A1
#define ledPin2 A2
#define ledPin3 A3
#define ledPin4 A4
#define ledPin5 A5

ezButton button1(7);
ezButton button2(2);
ezButton button3(13);
ezButton button4(12);
ezButton button5(11);

int currentLED = 1;
int nextLED;

static const uint8_t pinNumberOutput[] = {A1, A2, A3, A4, A5};

int lastToggle = millis();
int currentTime;
int LEDstate = LOW;

void toggleLED(int index){
  currentTime = millis();
  if(currentTime - lastToggle > 1000){
    if( LEDstate == LOW)
      LEDstate = HIGH;
    else
      LEDstate = LOW;
    lastToggle = millis();
    digitalWrite(pinNumberOutput[index],LEDstate);
  }
}

void newLED(){
    nextLED = random(1,5);
    if(nextLED == currentLED)
      newLED();
    currentLED = nextLED;
}

void setup() {
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  pinMode(ledPin4,OUTPUT);
  pinMode(ledPin5,OUTPUT);

  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  button4.setDebounceTime(50);
  button5.setDebounceTime(50);
}

void loop() {
//  button1.loop();
//  button2.loop();
//  button3.loop();
//  button4.loop();
//  button5.loop();
//
//  toggleLED(currentLED);
//
//
//  if(button1.isReleased() && currentLED == 1)
//    newLED();
//  else if(button2.isReleased() && currentLED == 2)
//    newLED();
//  else if(button3.isReleased() && currentLED == 3)
//    newLED();
//  else if(button4.isReleased() && currentLED == 4)
//    newLED();
//  else if(button5.isReleased() && currentLED == 5)
//    newLED();

  digitalWrite(ledPin5,HIGH);

}
