#include <SoftwareSerial.h>

#define rxPin 5
#define txPin 3

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  int readByte = mySerial.read();
  if (readByte == 5) {
    mySerial.write(7);
    Serial.println("Recieved trigger, sending confirmation");
  }
}
