//Modbus Setup
#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE8
};
IPAddress server(192, 168, 1, 1);
IPAddress ip(192, 168, 1, 2);
EthernetClient ethClient;
ModbusTCPClient modbusTCPClient(ethClient);

//Buttons setup
int enablePin = 7;    //Power button
int startPin = 8;     //play/pause button
int timeUpPin = 9;    //increase time
int timeDownPin = 10; //decrease time
int angle1Pin = 11;   //angle dial
int angle2Pin = 12;
int angle3Pin = 13;
int speed1Pin = A0;   //speed dial
int speed2Pin = A1;
int speed3Pin = A2;
int autoPin = A3;     //auto mode

// Initializing variables
int outputAngleValue = 1;
int outputSpeedValue = 1;
int time = 0;

enum dir {
  pos,
  neg
};

void setup() {
  //Buttons setup
  pinMode(enablePin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(timeUpPin, INPUT_PULLUP);
  pinMode(timeDownPin, INPUT_PULLUP);
  pinMode(angle1Pin, INPUT_PULLUP);
  pinMode(angle2Pin, INPUT_PULLUP);
  pinMode(angle3Pin, INPUT_PULLUP);
  pinMode(speed1Pin, INPUT_PULLUP);
  pinMode(speed2Pin, INPUT_PULLUP);
  pinMode(speed3Pin, INPUT_PULLUP);
  pinMode(autoPin, INPUT_PULLUP);
  Serial.begin(9600);

  //Modbus Setup
  Ethernet.init(10);   // MKR ETH shield
  Ethernet.begin(mac, ip);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
}

void loop() {
  //Modbus Loop
  if(!modbusTCPClient.connected()){
    Serial.println("Attempting to connect to Modbus TCP server");

    if (!modbusTCPClient.begin(server, 502)) {
      Serial.println("Modbus TCP Client failed to connect!");
    } else {
      Serial.println("Modbus TCP Client connected");
    }
  } else {
    
  }

  //Button Loop
  //while power button is ON
    //define angle, speed, time, axes, bounce
    //enable motors
    // home
    //how do we do these two only once? 
    //while play/pause is ON
      //start timer
      //if timer is less or equal to 0
      //ending sequence
    //remember the time we were at when paused

}

//set Angle levels
void setAngle(int lvl) {
  // if level 1 use setLimits for predefined value
  // if level 2 ...
  // if level 3 ...
}

//set limits
void setLimits(int lp, int ln) {
  //AXIS 1 Limits
  //108-109 is upper limit
  int address = 108;
  writeTwoRegisters(address,lp);
  //110-111 is lower limit
  address = 110;
  writeTwoRegisters(address,ln);
  //AXIS 2 Limits
  //124-125 is upper limit
  address = 124;
  writeTwoRegisters(address,lp);
  //126-127 is lower limit
  address = 126;
  writeTwoRegisters(address,ln);
}

void setVelLvl(int lvl){
  // if level 1 use setVelocity for predefined value
  // if level 2 ...
  // if level 3 ...
}

void setVelocity(int vel){
  //set velocity in ACTIONS(task parameters)
  //action 13 = neg, 14 = pos for AXIS 1 (address 112-113, 114-115)
  //action 15 = neg, 16 = pos for AXIS 2 (address 128-129, 130-131)
  int axis1neg = 112;
  int axis1pos = 114;
  int axis2neg = 128;
  int axis2pos = 130;
  writeTwoRegisters(axis1neg,-vel);
  writeTwoRegisters(axis1pos,vel);
  writeTwoRegisters(axis2neg,-vel);
  writeTwoRegisters(axis2pos,vel);
  //set general JOG velocities
  //AXIS1.JOG.V is at 118 and 119
  //AXIS1.JOG.V is at 120 and 121
  int axis1 = 118;
  int axis2 = 120;
  writeTwoRegisters(axis1,vel);
  writeTwoRegisters(axis2,vel);
}

void writeTwoRegisters(int address, int value){
  value = value*1000;
  int value1 = value/(16^4);
  int value2 = value - value1*(16^4);  
  modbusTCPClient.holdingRegisterWrite(address-1,value1);
  modbusTCPClient.holdingRegisterWrite(address,value2);
}

void enableAxes() {
  int axis1 = 96;
  int axis2 = 98;
  modbusTCPClient.holdingRegisterWrite(axis1-1,1); //set ENABLE to high, then IMMEDIATELY set it back to low
  modbusTCPClient.holdingRegisterWrite(axis1-1,0);
  modbusTCPClient.holdingRegisterWrite(axis2-1,1);
  modbusTCPClient.holdingRegisterWrite(axis2-1,0);
}

void disableAxes() {
  int axis1 = 97;
  int axis2 = 99;
  modbusTCPClient.holdingRegisterWrite(axis1-1,1); //set ENABLE to high, then IMMEDIATELY set it back to low
  modbusTCPClient.holdingRegisterWrite(axis1-1,0);
  modbusTCPClient.holdingRegisterWrite(axis2-1,1);
  modbusTCPClient.holdingRegisterWrite(axis2-1,0);
}

void jog(int axis, dir pn) { // will need to specify which axis and pos/neg
  int address = 100;
  if(axis == 2){
    int address = 101;
  }
  if(pn == neg){
    address = address + 1;
  }
  modbusTCPClient.holdingRegisterWrite(address-1,1); //set ENABLE to high, then IMMEDIATELY set it back to low
  modbusTCPClient.holdingRegisterWrite(address-1,0);
}

int actionRunning(){
  int address1 = 104;
  int address2 = 105;
  int data1 = modbusTCPClient.holdingRegisterRead(address1);
  int data2 = modbusTCPClient.holdingRegisterRead(address2);
  int result = (data1 << 16) + data2;
  return result;
}

void enableCmdSource(){
  int address = 116;
  modbusTCPClient.holdingRegisterWrite(address-1,1);
  modbusTCPClient.holdingRegisterWrite(address,1);
}

void enableActions(){ //should only need to do this once at start
  int address = 106;  // for ACTION 13 & 14
  modbusTCPClient.holdingRegisterWrite(address-1,1);
  modbusTCPClient.holdingRegisterWrite(address,1);
  address = 122;      // for ACTION 15 & 16
  modbusTCPClient.holdingRegisterWrite(address-1,1);
  modbusTCPClient.holdingRegisterWrite(address,1);
}
