#include <SPI.h>
#include <SCL3300.h>
#include <SoftwareSerial.h>

#define rxPin 5
#define txPin 3

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

int axis1_down = 7;
int axis1_up = 6;
int both_down = 5;
int both_up = 4;
int both_stop = 3;

double roll = 0;
double pitch =0;
int home_status =0;

// Initialize Inclinometer
SCL3300 inclinometer;

#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  #define Serial SERIAL_PORT_USBVIRTUAL
  #endif

  
// Function to get angle data
void getAngles(){
  if(inclinometer.available()){
    pitch = inclinometer.getTiltLevelOffsetAngleX();  // pitch value is saved
    roll = inclinometer.getTiltLevelOffsetAngleY();   // roll value is saved
  }
  else inclinometer.reset();
}

void rollHoming(){
  getAngles();
  mySerial.write(axis1_down);
  while(roll < 0){
    // move axis 1 down
    Serial.println(roll);
    getAngles();
  }
  mySerial.write(both_stop); // Stop motor
  mySerial.write(axis1_up);
  while(roll > 0){
    // move axis 1 up
    Serial.println(roll);
    getAngles();
  }
  mySerial.write(both_stop); // Stop motor
}

void pitchHoming(){
  getAngles();
  mySerial.write(both_up);
  while(pitch < 0){
    // move both axes up
    Serial.println(pitch);
    getAngles();
  }
  mySerial.write(both_stop); // Stop motor
  mySerial.write(both_down);
  while(pitch > 0){
    // move both axes down
    Serial.println(pitch);
    getAngles();
  }
  mySerial.write(both_stop); // Stop motor
}

  

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(2000); //SAMD boards may need a long time to init SerialUSB
  Serial.println("Reading basic Tilt values from SCL3300 Inclinometer");

  if (inclinometer.begin() == false) {
    Serial.println("Murata SCL3300 inclinometer not connected.");
    while(1); //Freeze
  }
  

}

void loop() {
  getAngles();
  while(abs(roll) > 0.5){
    rollHoming();
  }
  while(abs(pitch) > 0.5){
    pitchHoming();
  }

    if(abs(roll) < 0.5 && abs(pitch) < 0.5){
    Serial.println("Finished Roll"); // set the current position values to 0 degrees
    mySerial.write(both_stop);
    }
}
