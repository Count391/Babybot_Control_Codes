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
int nextIndex = 0;
int pinNumberInput[] = {11, 12, 13, 2, 7};
static const uint8_t pinNumberOutput[] = {A1, A2, A3, A4, A5};
unsigned long previousMillis = 0;
unsigned long currentMillis;

void setup() {
  // Setting up input and output pins
  while (int i = 0 < 5) {
    pinMode(pinNumberInput[i], INPUT_PULLUP);
    pinMode(pinNumberOutput[i], OUTPUT);
    i++;
  }
  
  // Turning on all lights for initial check
  for(int i = 0; i <= 1; i++){
    for (int j = 0; j < 5; j++) {
      digitalWrite(pinNumberOutput[j], HIGH);
      delay(1000);
      digitalWrite(pinNumberOutput[j],LOW);
      delay(1000);
    }
  }
  
  // Generating random lighting pattern
  randomSeed(analogRead(A0));
  nextIndex = random(0,5);
  nextLED = pinNumberOutput[nextIndex];
  
  // Adafruit VS1053 Simple Test 
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // Setting up VS1053
  musicPlayer.setVolume(20,20);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
  Serial.println(F("Playing background music"));
  musicPlayer.playFullFile("/track001.mp3");
}

void loop() {
  // LED button pattern (blinking)
  currentMillis = millis();
  if ((currentLED != nextLED) && ((previousMillis - currentMillis) > 500)){  
    if (digitalRead(currentLED) == 0){
      digitalWrite((currentLED), HIGH);
    }
    else{ 
      digitalWrite((currentLED), LOW);
    }
    previousMillis = currentMillis;
  }

            
  // Button is correctly pressed
  int i = 0;
  while (i < 5) {
    if (!digitalRead(pinNumberInput[i]) && currentLED == (pinNumberOutput[i])){
      // Play successful music
      musicPlayer.stopPlaying();
      musicPlayer.playFullFile("/track002.mp3");
      // Determine next button
      digitalWrite((currentLED), LOW);
      while (currentLED == nextLED) {
        nextIndex = random(0,5);
        nextLED = pinNumberOutput[nextIndex];
      } // Resume background music
      musicPlayer.playFullFile("/track001.mp3");
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
