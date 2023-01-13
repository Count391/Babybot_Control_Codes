int currentLED = 0;
int nextLED = 0;

void setup() {
  int pinNumber = 22;
  while(pinNumber <= 27){
    pinMode(pinNumber, INPUT_PULLUP); 
    pinNumber = pinNumber + 1;
  }      
  while(pinNumber <= 33){
    pinMode(pinNumber, OUTPUT);
    pinNumber = pinNumber + 1;
  }
  randomSeed(analogRead(A0)); 
  nextLED = random(1, 7);
}

void loop() {
  if(currentLED != nextLED){
    digitalWrite((currentLED + 27), LOW);
    delay(250);
    digitalWrite((currentLED + 27), HIGH);
    delay(1000);
    digitalWrite((currentLED + 27), LOW);
    delay(250);
    digitalWrite((currentLED + 27), HIGH);
    delay(1000);
    digitalWrite((currentLED + 27), LOW);
    currentLED = nextLED;
    digitalWrite((currentLED + 27), HIGH);
  }
  if(!digitalRead(22)||currentLED == 1){
    nextLED = random(1, 7);
  }else if(!digitalRead(23)||currentLED == 2){
    nextLED = random(1, 7);
  }else if(!digitalRead(24)||currentLED == 3){
    nextLED = random(1, 7);
  }else if(!digitalRead(25)||currentLED == 4){
    nextLED = random(1, 7);
  }else if(!digitalRead(26)||currentLED == 5){
    nextLED = random(1, 7);
  }else if(!digitalRead(27)||currentLED == 6){
    nextLED = random(1, 7);
  }
}
