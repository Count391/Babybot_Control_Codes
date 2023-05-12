#include <ezButton.h>

#define ledPin1 A5 // Green
#define ledPin2 A4 // Red
#define ledPin3 A3 // White
#define ledPin4 A2 // Blue
#define ledPin5 A1 // Yellow

ezButton button1(7); // Green
ezButton button2(2); // Red
ezButton button3(13); // White
ezButton button4(12); // Blue
ezButton button5(11); // Yellow

int currentLED = 1;
int nextLED;

static const uint8_t pinNumberOutput[] = {A5, A4, A3, A2, A1};

int lastToggle = millis();
int currentTime;
int LEDstate = LOW;

void toggleLED(){
  currentTime = millis();
  if(currentTime - lastToggle > 1000){
    if(LEDstate == LOW)
      LEDstate = HIGH;
    else
      LEDstate = LOW;
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
  Serial.print("Current button: ");
  Serial.println(currentLED);
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

// currently not toggling lights on/off  
  
if(button1.isPressed() && currentLED == 1){
  Serial.println("The button 1 is released");
  digitalWrite(ledPin1,LOW);
  newLED(); 
  digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
  Serial.print("Current button: ");
  Serial.println(currentLED);
}

if(button2.isPressed() && currentLED == 2){
  Serial.println("The button 2 is released");
  digitalWrite(ledPin2,LOW);
  newLED();
  digitalWrite(pinNumberOutput[currentLED-1],HIGH);
  Serial.print("Current button: ");
  Serial.println(currentLED);
}

if(button3.isPressed() && currentLED == 3){
  Serial.println("The button 3 is released");
  digitalWrite(ledPin3,LOW);
  newLED();
  digitalWrite(pinNumberOutput[currentLED-1],HIGH);
  Serial.print("Current button: ");
  Serial.println(currentLED);
}

if(button4.isPressed() && currentLED == 4){
  Serial.println("The button 4 is released");
  digitalWrite(ledPin4,LOW);
  newLED();
  digitalWrite(pinNumberOutput[currentLED-1],HIGH);
  Serial.print("Current button: ");
  Serial.println(currentLED);
}

if(button5.isPressed() && currentLED == 5){
  Serial.println("The button 5 is released");
  digitalWrite(ledPin5,LOW);
  newLED();
  digitalWrite(pinNumberOutput[currentLED-1],HIGH);
  Serial.print("Current button: ");
  Serial.println(currentLED);
}
}
