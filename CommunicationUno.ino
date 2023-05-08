// for Uno
#include <ezButton.h>
#include <SoftwareSerial.h>

#define rxPin 5
#define txPin 3

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

char mystr[10] = "Tilt"; //String data
char mystr2[10] = "Roll"; //String data
char mystr3[10] = "Combined"; //String data

ezButton axis(9);  // axis
int axis_state = 1;

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  mySerial.begin(9600);
  axis.setDebounceTime(50);
}

void loop() {
  axis.loop();
  if (axis.isReleased()){
    if (axis_state == 1){
      mySerial.write(mystr,10);
      axis_state = 2;
    }
    else  if (axis_state == 2){
      mySerial.write(mystr2,10);
      axis_state = 3;
    }
    else{
      mySerial.write(mystr3,10);
      axis_state = 1;
    }
  }
//  if (axis.isReleased()){
//    if (axis_state == 1){
//      Serial.println("Combined");
//      axis_state = 2;
//    }
//    else  if (axis_state == 2){
//      Serial.println("Pitch");
//      axis_state = 3;
//    }
//    else{
//      Serial.println("Roll");
//      axis_state = 1;
//    }
//  }
    //mySerial.write(3);
    //mySerial.write(mystr,10); //Write the serial data
    //delay(1000);
    //mySerial.write(3);
    //mySerial.write(mystr2,10);
    //mySerial.write(mystr3,10);
}
