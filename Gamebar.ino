int currentLED = 0;                             //Variable for next LED display instruction
int nextLED = 0;                                //Variable for current LED display instruction

void setup() {
  int pinNumber = 2;
  while(pinNumber <= 6){
    pinMode(pinNumber, INPUT_PULLUP); 
    pinNumber++;
  }                                             //Set the input pins
  while(pinNumber <= 11){
    pinMode(pinNumber, OUTPUT);
    pinNumber++;
  }                                             //Set the output pins
  int i = 0;
  for(i = 0; i <= 1; i++){                      //Turn on all lights for initial check
    for(pinNumber = 7; pinNumber <= 11; pinNumber++){
      digitalWrite(pinNumber, HIGH); 
    }
    delay(1000);
    for(pinNumber = 7; pinNumber <= 11; pinNumber++){
      digitalWrite(pinNumber, LOW); 
    }
    delay(1000);
  }
  randomSeed(analogRead(A0));                   //Generate random lighting pattern
  nextLED = random(7, 12);                       //Initialize the LED pattern
  Serial.begin(9600);
}

void loop() {
  if(currentLED != nextLED){                    //Blinking for the LED pattern change
    digitalWrite((currentLED), LOW);
    delay(250);
    digitalWrite((currentLED), HIGH);
    delay(1000);
    digitalWrite((currentLED), LOW);
    delay(250);
    digitalWrite((currentLED), HIGH);
    delay(1000);
    digitalWrite((currentLED), LOW);
    currentLED = nextLED;
    digitalWrite((currentLED), HIGH);
  }
  Serial.println(currentLED);
  if(!digitalRead(2) && currentLED == 7){        //Check input against target
    nextLED = random(7, 12);                     //Change pattern if input matches target
  }else if(!digitalRead(3) && currentLED == 8){
    nextLED = random(7, 12);
  }else if(!digitalRead(4) && currentLED == 9){
    nextLED = random(7, 12);
  }else if(!digitalRead(5) && currentLED == 10){
    nextLED = random(7, 12);
  }else if(!digitalRead(6) && currentLED == 11){
    nextLED = random(7, 12);
  }
}
