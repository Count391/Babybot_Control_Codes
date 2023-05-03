// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)


// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =   Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

int currentLED = 0;                             //Variable for next LED display instruction
int nextLED = 0;                                //Variable for current LED display instruction
unsigned long testMillis;
unsigned long currentMillis;

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
  
  for(int i = 0; i <= 1; i++){                  //Turn on all lights for initial check
    for (pinNumber = 7; pinNumber <= 11; pinNumber++) {
      testMillis = millis()
      while (testMillis < 1000){
        digitalWrite(pinNumber, HIGH);
      }
      while (testMillis <2000){
        digitalWrite(pinNumber,LOW)       
      }
    }
  }
  randomSeed(analogRead(A0));                   //Generate random lighting pattern
  nextLED = random(7, 12);                       //Initialize the LED pattern

  
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
  musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
  Serial.println(F("Playing background music"));
  musicPlayer.playFullFile("/track001.mp3");
}

void loop() {
  // Blinking for the LED pattern change
  if (currentLED != nextLED){  
    // Toggling High/Low 5 times (1.25 seconds)
    for (int i = 0, i < 5, i++) {
      currentMillis = millis();
      while (currentMillis < 250){
        digitalWrite((currentLED), LOW);
      }
      while (currentMillis < 1250){
        digitalWrite((currentLED, HIGH);
      }
    }
    digitalWrite((currentLED), LOW);
    currentLED = nextLED;
    digitalWrite((currentLED), HIGH);
  }
  Serial.println(currentLED);

  // Changing Pattern if Input Matches Target
  int i = 2;
  while (i < 7) {
    if (!digitalRead(i) && currentLED == (i+5)){
      musicPlayer.stopPlaying();
      musicPlayer.playFullFile("/track002.mp3"); // Successful Music
      nextLED = random(7,12);
      // Confirming nextLED is Different
      while (currentLED == nextLED) {
        nextLED = random(7,12);
      }
      if (musicPlayer.stopped()){
        musicPlayer.playFullFile("/track001.mp3"); // Background Music
      }
    }
    i++;
  }

  // Replaying Background Music if Finished
  if (musicPlayer.stopped()){
    musicPlayer.playFullFile("/track001.mp3");
  }
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
