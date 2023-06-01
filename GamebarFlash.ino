#include <ezButton.h>

#define ledPin1 A5 // Green
#define ledPin2 A4 // Red
#define ledPin3 A2 // Blue
#define ledPin4 A1 // Yellow

ezButton button1(7); // Green
ezButton button2(2); // Red
ezButton button3(A0); // Blue
ezButton button4(A3); // Yellow

int currentLED = 1;
int nextLED;

static const uint8_t pinNumberOutput[] = {A5, A4, A2, A1};

int lastToggle = millis();
int currentTime;
int currentTimer;
int LEDstate = LOW;
int count = 0;

void toggleLED(int duration){
  currentTime = millis();
  if(currentTime - lastToggle > duration){
    if(LEDstate == LOW) {
      LEDstate = HIGH;
      count++;
    }
    else {
      LEDstate = LOW;
      count++;
    }
    lastToggle = millis();
    digitalWrite(pinNumberOutput[currentLED-1],LEDstate);
  }
}

void newLED(){
    nextLED = random(1,5);
    while(nextLED == currentLED)
      nextLED = random(1,5);
    currentLED = nextLED;
}

void setup() {
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  pinMode(ledPin4,OUTPUT);

  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  button4.setDebounceTime(50);

  Serial.begin(9600);
  Serial.print("Next button: ");
  Serial.println(currentLED);
  digitalWrite(pinNumberOutput[currentLED-1],HIGH);
}

void loop() {

button1.loop();
button2.loop();
button3.loop();
button4.loop();

int btn1State = button1.getState();
int btn2State = button2.getState();
int btn3State = button3.getState();
int btn4State = button4.getState();

if(button1.isPressed() && currentLED == 1){
  digitalWrite(ledPin1,LOW);
  newLED(); 
  digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
  Serial.print("Next Button:");
  Serial.println(currentLED);
}

if(button1.isPressed() && currentLED != 1){
  currentTimer = millis();
  if(currentTimer - lastToggle > 1000){  
    Serial.print("Wrong Button, Press ");
    Serial.println(currentLED);
    while (count < 20){
      toggleLED(150);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}
if(button2.isPressed() && currentLED == 2){
  digitalWrite(ledPin2,LOW);
  newLED(); 
  digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
  Serial.print("Next Button:");
  Serial.println(currentLED);
}

if(button2.isPressed() && currentLED != 2){
  currentTimer = millis();
  if(currentTimer - lastToggle > 1000){  
    Serial.print("Wrong Button, Press ");
    Serial.println(currentLED);
    while (count < 20){
      toggleLED(150);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}

if(button3.isPressed() && currentLED == 3){
  digitalWrite(ledPin3,LOW);
  newLED(); 
  digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
  Serial.print("Next Button:");
  Serial.println(currentLED);
}

if(button3.isPressed() && currentLED != 3){
  currentTimer = millis();
  if(currentTimer - lastToggle > 1000){  
    Serial.print("Wrong Button, Press ");
    Serial.println(currentLED);
    while (count < 20){
      toggleLED(150);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}

if(button4.isPressed() && currentLED == 4){
  digitalWrite(ledPin4,LOW);
  newLED(); 
  digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
  Serial.print("Next Button:");
  Serial.println(currentLED);
}

if(button4.isPressed() && currentLED != 4){
  currentTimer = millis();
  if(currentTimer - lastToggle > 1000){  
    Serial.print("Wrong Button, Press ");
    Serial.println(currentLED);
    while (count < 20){
      toggleLED(150);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}
}
