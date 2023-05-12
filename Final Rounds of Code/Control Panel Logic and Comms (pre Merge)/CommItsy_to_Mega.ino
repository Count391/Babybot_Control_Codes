// Comms test for itsy
#include <ezButton.h>
#include <SoftwareSerial.h>

#define rxPin 0
#define txPin 1

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

int my_data = 45;
int my_data2 = 25;

ezButton buttn(11);  // button
int state = 1; 

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  mySerial.begin(9600);
  buttn.setDebounceTime(50);
}

void loop() {
  buttn.loop();
  if(buttn.isReleased() && state == 1) {
    mySerial.write(my_data);
    state = 0;
  }
  else if(buttn.isReleased() && state == 0) {
    mySerial.write(my_data2);
    state = 1;
  }
}
