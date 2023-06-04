/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SoftwareSerial.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define rxPin 5
#define txPin 6
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

#define ledPin1 A5 // Green
#define ledPin2 A4 // Red
#define ledPin3 A2 // Blue
#define ledPin4 A1 // Yellow
#define redPin 7
#define greenPin 2

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  //Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

int currentLED = 1;
int nextLED;
long bounceTime = 0;

static const uint8_t pinNumberOutput[] = {A5, A4, A2, A1};

long lastToggle = millis();
long currentTime;
long currentTimer;
int LEDstate = LOW;
int count = 0;
int buttonPressed = 0;

void toggleLED(long duration){
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

void gameResp(){
  if (buttonPressed == 0){
    return;
  }else if(buttonPressed == currentLED){
    mySerial.write(10);
    musicPlayer.stopPlaying();
    digitalWrite(pinNumberOutput[currentLED-1],LOW); 
    newLED(); 
    digitalWrite(pinNumberOutput[currentLED-1],HIGH); 
    Serial.print("Next Button:");
    Serial.println(currentLED);
    musicPlayer.startPlayingFile("/correct.mp3");
  }else if(buttonPressed != currentLED){
    mySerial.write(11);
    musicPlayer.stopPlaying();
    musicPlayer.startPlayingFile("/wrong.mp3");
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
  bounceTime = millis() + 1250;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(5,5);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
  Serial.println(F("Playing track 002"));
  musicPlayer.startPlayingFile("/bgm.mp3");
  pinMode(A0, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(redPin, INPUT_PULLUP);
  pinMode(greenPin, INPUT_PULLUP);
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  pinMode(ledPin4,OUTPUT);
  Serial.print("Next button: ");
  Serial.println(currentLED);
  digitalWrite(pinNumberOutput[currentLED-1],HIGH);
}



void loop() {
  // File is playing in the background
  if(bounceTime < millis()){
    if (musicPlayer.stopped()) {
      musicPlayer.startPlayingFile("/bgm.mp3");
      Serial.println("bgm is on");
    }
    if (!(digitalRead(redPin))){
      buttonPressed = 1;
      Serial.println("Button 1 Pressed");
    }else if(!(digitalRead(greenPin))){
      buttonPressed = 2;
      Serial.println("Button 2 Pressed");
    }else if(!(digitalRead(A0))){
      buttonPressed = 3;
      Serial.println("Button 3 Pressed");
    }else if(!(digitalRead(A3))){
      buttonPressed = 4;
      Serial.println("Button 4 Pressed");
    }
  }

  gameResp();
}


/// File listing helper
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
