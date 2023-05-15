#include <ezButton.h>

#define ledPin1 A5 // Green
#define ledPin2 A4 // Red
#define ledPin3 A3 // White
#define ledPin4 A2 // Blue
#define ledPin5 A1 // nothing

ezButton button1(7); // Green
ezButton button2(2); // Red
ezButton button3(13); // White but not responding to press
ezButton button4(12); // Blue
ezButton button5(11); // Yellow

int currentLED = 1;
int nextLED;

static const uint8_t pinNumberOutput[] = {A5, A4, A3, A2, A1};

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
    nextLED = random(1,6);
    while(nextLED == currentLED)
      nextLED = random(1,6);
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
button5.loop();

int btn1State = button1.getState();
int btn2State = button2.getState();
int btn3State = button3.getState();
int btn4State = button4.getState();
int btn5State = button5.getState();

if(button1.isPressed() && currentLED == 1){
  while (count < 10){
    toggleLED(400);
  }
  count = 0;
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
      toggleLED(100);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}
if(button2.isPressed() && currentLED == 2){
  while (count < 10){
    toggleLED(400);
  }
  count = 0;
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
      toggleLED(100);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}

if(button3.isPressed() && currentLED == 3){
  while (count < 10){
    toggleLED(400);
  }
  count = 0;
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
      toggleLED(100);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}

if(button4.isPressed() && currentLED == 4){
  while (count < 10){
    toggleLED(400);
  }
  count = 0;
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
      toggleLED(200);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}

if(button5.isPressed() && currentLED == 5){
  while (count < 10){
    toggleLED(400);
  }
  count = 0;
  digitalWrite(ledPin5,LOW);
  newLED(); 
  digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
  Serial.print("Next Button:");
  Serial.println(currentLED);
  }

if(button5.isPressed() && currentLED != 5){
  currentTimer = millis();
  if(currentTimer - lastToggle > 1000){  
    Serial.print("Wrong Button, Press ");
    Serial.println(currentLED);
    while (count < 20){
      toggleLED(100);
    }    
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    count = 0;
  }
}
}
