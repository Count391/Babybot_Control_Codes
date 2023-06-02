#include <ezButton.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SoftwareSerial.h>

#define rxPin 5
#define txPin 6
#define ledPin1 A5 // Green
#define ledPin2 A4 // Red
#define ledPin3 A2 // Blue
#define ledPin4 A1 // Yellow
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =   Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

ezButton button1(7); // Green
ezButton button2(2); // Red

int currentLED = 1;
int nextLED;

static const uint8_t pinNumberOutput[] = {A5, A4, A2, A1};

int lastToggle = millis();
int currentTime;
int currentTimer;
int LEDstate = LOW;
int count = 0;
int buttonPressed = 0;

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
    while(nextLED == currentLED){
      nextLED = random(1,5);
    }
    currentLED = nextLED;
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void gameResp(){
  if (buttonPressed == 0){
    return;
  }else if(buttonPressed == currentLED){
    mySerial.write(10);
    digitalWrite(pinNumberOutput[currentLED-1],LOW); 
    newLED(); 
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    Serial.print("Next Button:");
    Serial.println(currentLED);
    musicPlayer.playFullFile("/correct.mp3");
    Serial.println("Playing correct sound");
  }else if(buttonPressed != currentLED){
    mySerial.write(11);
    musicPlayer.playFullFile("/wrong.mp3");
    Serial.println("Playing wrong music");
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
  buttonPressed = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);

  while (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     delay(1000);
  }
  Serial.println(F("VS1053 found"));
  
  while (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    delay(1000);
  }
  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(1,1);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(3);  // DREQ int
  
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  pinMode(ledPin4,OUTPUT);

  button1.setDebounceTime(50);
  button2.setDebounceTime(50); 


  //Serial.begin(9600);
  Serial.print("Next button: ");
  Serial.println(currentLED);
  digitalWrite(pinNumberOutput[currentLED-1],HIGH);
}

void loop() {
  if (musicPlayer.stopped()) {
    musicPlayer.startPlayingFile("/bgm.mp3");
    Serial.println("Playing bg music");
  }
  
  button1.loop();
  button2.loop();
  
  if (button1.isPressed()){
    buttonPressed = 1;
    Serial.println("Button 1 Pressed");
  }else if(button2.isPressed()){
    buttonPressed = 2;
    Serial.println("Button 2 Pressed");
  }else if(!(digitalRead(A0))){
    buttonPressed = 3;
    delay(250);
    Serial.println("Button 3 Pressed");
  }else if(!(digitalRead(A3))){
    buttonPressed = 4;
    delay(250);
    Serial.println("Button 4 Pressed");
  }

  gameResp();
}
