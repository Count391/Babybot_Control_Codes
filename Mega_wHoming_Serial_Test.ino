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

enum dir {
  pos,
  neg
};

// Initialize variables for incoming data
int incoming_data;
int axis1_down = 7;
int axis1_up = 6;
int both_down = 5;
int both_up = 4;
int both_stop = 3;


void setup() {
  //Buttons setup
  Serial2.begin(9600);
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
      enableAxes();
      setVelLvl(2);
      setAngle(2);
    }
  } else {
    
  }

// Read Serial Comms from Uno (Inclinometer)
//
//
//
//
//
while(!Serial2.available()){
    
  }
  incoming_data = Serial2.read();
  setVelocity(100);
  if(incoming_data == axis1_down){
    Serial.println("Axis 1 Down");
    enableAxes();
    delay(50);
    jog(1,neg);
  }
  else if(incoming_data == axis1_up){
    Serial.println("Axis 1 Up");
    enableAxes();
    delay(50);
    jog(1,pos);
  }
  else if(incoming_data == both_up){
    Serial.println("Both Up");
    enableAxes();
    delay(50);
    jog(1,pos);
    delay(50);
    jog(2,pos);
    Serial.println(incoming_data);
  }
  else if(incoming_data == both_down){
    Serial.println("Both Down");
    enableAxes();
    delay(50);
    jog(1,neg);
    delay(50);
    jog(2,neg);
  }
  else if(incoming_data == both_stop){
    Serial.println("Both Stop");
    disableAxes();
  }

}


// Functions for Kollmorgen
//set Angle levels
void setAngle(int lvl) {
  // if level 1 use setLimits for predefined value
  // if level 2 ...
  // if level 3 ...
  if (lvl == 1) {
    setLimits(1500, 2000);
  }
  else if (lvl == 2) {
    setLimits(750, 2000);
  }
  else if (lvl == 3) {
    setLimits(100, 2000);
  }
}

//set limits
void setLimits(int ln, int lp) {
  //AXIS 1 Limits
  //108-109 is upper limit
  int address = 108;
  writeTwoRegisters(address, lp);
  //110-111 is lower limit
  address = 110;
  writeTwoRegisters(address, ln);
  //AXIS 2 Limits
  //124-125 is upper limit
  address = 124;
  writeTwoRegisters(address, lp);
  //126-127 is lower limit
  address = 126;
  writeTwoRegisters(address, ln);
  enableActions(); // ACTIONS disable whenever a parameter is changed
}

void setVelLvl(int lvl) {
  // if level 1 use setVelocity for predefined value
  // if level 2 ...
  // if level 3 ...
  if (lvl == 1) {
    setVelocity(300);
  }
  else if (lvl == 2) {
    setVelocity(800);
  }
  else if (lvl == 3) {
    setVelocity(1200);
  }
}

void setVelocity(int vel) {
  //set velocity in ACTIONS(task parameters)
  //action 13 = neg, 14 = pos for AXIS 1 (address 112-113, 114-115)
  //action 15 = neg, 16 = pos for AXIS 2 (address 128-129, 130-131)
  int axis1neg = 112;
  int axis1pos = 114;
  int axis2neg = 128;
  int axis2pos = 130;
  writeTwoRegisters(axis1neg, -vel);
  writeTwoRegisters(axis1pos, vel);
  writeTwoRegisters(axis2neg, -vel);
  writeTwoRegisters(axis2pos, vel);
  //set general JOG velocities
  //AXIS1.JOG.V is at 118 and 119
  //AXIS1.JOG.V is at 120 and 121
  int axis1 = 118;
  int axis2 = 120;
  writeTwoRegisters(axis1, vel);
  writeTwoRegisters(axis2, vel);
  enableActions();  // ACTIONS disable whenever a parameter is changed
}

void writeTwoRegisters(int address, int value) {
  long v = value * 1000.00;
  long value1 = v >> 16;
  long value2 = v - (value1 << 16);
  modbusTCPClient.holdingRegisterWrite(address - 1, int(value1));
  modbusTCPClient.holdingRegisterWrite(address, int(value2));
}

void enableAxes() {
  int axis1 = 96;
  int axis2 = 98;
  modbusTCPClient.holdingRegisterWrite(axis1 - 1, 1); //set ENABLE to high, then IMMEDIATELY set it back to low
  modbusTCPClient.holdingRegisterWrite(axis1 - 1, 0);
  modbusTCPClient.holdingRegisterWrite(axis2 - 1, 1);
  modbusTCPClient.holdingRegisterWrite(axis2 - 1, 0);
}

void disableAxes() {
  int axis1 = 97;
  int axis2 = 99;
  modbusTCPClient.holdingRegisterWrite(axis1 - 1, 1); //set ENABLE to high, then IMMEDIATELY set it back to low
  modbusTCPClient.holdingRegisterWrite(axis1 - 1, 0);
  modbusTCPClient.holdingRegisterWrite(axis2 - 1, 1);
  modbusTCPClient.holdingRegisterWrite(axis2 - 1, 0);
}

void jog(int axis, dir pn) { // will need to specify which axis and pos/neg
  int address = 100;
  if (axis == 2) {
    address = 102;
  }
  if (pn == neg) {
    address = address + 1;
  }
  modbusTCPClient.holdingRegisterWrite(address - 1, 1); //set ENABLE to high, then IMMEDIATELY set it back to low
  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
}

int actionRunning() {
  int address1 = 104;
  int address2 = 105;
  int data1 = modbusTCPClient.holdingRegisterRead(address1);
  int data2 = modbusTCPClient.holdingRegisterRead(address2);
  int result = (data1 << 16) + data2;
  return result;
}

void enableCmdSource() {
  int address = 116;
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
}


//MUST ENABLE ACTIONS AFTER CHANGING PARAMETER OR CONDITION VALUE
void enableActions() { //should only need to do this once at start
  int address = 106;  // for ACTION 13 & 14
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
  address = 122;      // for ACTION 15 & 16
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
}

void toggleMotion(){
  enableAxes();
  delay(50);
  jog(1,pos);
  jog(2,pos);
}
