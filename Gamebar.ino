int currentLED = 0;                             //Variable for next LED display instruction
int nextLED = 0;                                //Variable for current LED display instruction

void setup() {
  int pinNumber = 22;
  while(pinNumber <= 26){
    pinMode(pinNumber, INPUT_PULLUP); 
    pinNumber++;
  }                                             //Set the input pins
  while(pinNumber <= 31){
    pinMode(pinNumber, OUTPUT);
    pinNumber++;
  }                                             //Set the output pins
  randomSeed(analogRead(A0));                   //Generate random lighting pattern
  nextLED = random(1, 6);                       //Initialize the LED pattern
}

void loop() {
  if(currentLED != nextLED){                    //Blinking for the LED pattern change
    digitalWrite((currentLED + 26), LOW);
    delay(250);
    digitalWrite((currentLED + 26), HIGH);
    delay(1000);
    digitalWrite((currentLED + 26), LOW);
    delay(250);
    digitalWrite((currentLED + 26), HIGH);
    delay(1000);
    digitalWrite((currentLED + 26), LOW);
    currentLED = nextLED;
    digitalWrite((currentLED + 26), HIGH);
    digitalWrite
  }
  if(!digitalRead(22)&&currentLED == 1){        //Check input against target
    nextLED = random(1, 6);                     //Change pattern if input matches target
  }else if(!digitalRead(23)&&currentLED == 2){
    nextLED = random(1, 6);
  }else if(!digitalRead(24)&&currentLED == 3){
    nextLED = random(1, 6);
  }else if(!digitalRead(25)&&currentLED == 4){
    nextLED = random(1, 6);
  }else if(!digitalRead(26)&&currentLED == 5){
    nextLED = random(1, 6);
  }else if(!digitalRead(27)&&currentLED == 6){
    nextLED = random(1, 6);
  }
}
