#include <ezButton.h>
#include <SoftwareSerial.h>

#define rxPin 5
#define txPin 3

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

ezButton trigger(8);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  trigger.setDebounceTime(50);
}

void loop() {
  trigger.loop();
  
  if (trigger.isReleased()) {
    mySerial.write(5);
    Serial.println("Sent trigger");
  }
  if (mySerial.read() == 7) {
    Serial.println("Recieved confirmation");
  }
}
