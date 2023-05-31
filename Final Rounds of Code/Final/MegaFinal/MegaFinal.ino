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

int playstate = 1;

// Initialize variables for incoming data
int incoming_data;
int axis1_down = 7;
int axis1_up = 6;
int axis2_down = 8;
int axis2_up = 9;
int both_down = 5;
int both_up = 4;
int both_stop = 3;
int homed = 1;
int initiate_home = 2;
int mode = 0;
int axis;
int spd;
int limitp;
int limitn;


void setup() {
  //Buttons setup
  //Serial1.begin(9600);  // This is for the gamebar
  Serial2.begin(9600);  // This is for the control panel
  Serial3.begin(9600);  // This is for the tilt sensor
  Serial.begin(9600);
  delay(20000);
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
      disableAxes();
      clearModbusErr();
    }
  } else {
    
  }

  incoming_data = Serial2.read();
  if(initiate_home == 2) {            // If the control panel sends command to find home
    Serial3.write(initiate_home);
    Serial.println("Sending command to start home to Uno"); 
    disableLimits();
    enableAxes();
    while(incoming_data !=1) {
      incoming_data = Serial3.read();
      if(incoming_data != -1)
        Serial.println(incoming_data);
      checkHomingMove();
    }
    setCurrentPosZeroAngle();
    Serial.println("Homing finished");
    initiate_home = 1;
    Serial3.write(initiate_home);
  }
  disableLimits();
   
  if(incoming_data/10 == 1) {
    Serial.print("Axis is: ");
    Serial.println(incoming_data%10);
    axis = incoming_data%10;
    go2zeroDegrees();             // If the axis, angle or speed changes, go to zero
  }
  if(incoming_data/10 == 2) {
    Serial.print("Angle is: ");
    Serial.println(incoming_data%10);
    setAngle(incoming_data%10);
    go2zeroDegrees();
  }
  if(incoming_data/10 == 3) {
    Serial.print("Speed is: ");
    Serial.println(incoming_data%10);
    spd = incoming_data%10;
    setVelLvl(spd);
  }
  if(incoming_data/10 == 4) {
    Serial.print("Mode is: ");
    Serial.println(incoming_data%10);
    mode = incoming_data%10;
  }
  if(incoming_data == 52) {
    Serial.println("Start BabyBot");
    if (axis == 0) {
      //combined
      Serial.println("Combined");
      setComboVelLvl(spd);
      toggleMotion(pos,neg);
      // do more work here
    }
    else if (axis == 1) {
      //pitch
      Serial.println("Pitch");
      setVelLvl(spd);
      toggleMotion(pos,pos);
    }
    else {
      // roll
      Serial.println("Roll");
      setVelLvl(spd);
      toggleMotion(pos,neg);
    }
  }
  if(incoming_data == 51) {
    Serial.println("Stop BabyBot");
    stopJog();
    delay(500);
    stopJog();
    delay(500);
    stopJog();
  }
  if(incoming_data == 50) {
    Serial.println("BabyBot is off");
    disableAxes();
  }
  if(incoming_data == 53) {
    Serial.println("Home the BabyBot");
    go2zeroDegrees();
  }

}
//
//
//
//
//
// Functions for Kollmorgen
//set Angle levels
void setAngle(int lvl) {
  // if level 1 use setLimits for predefined value
  // if level 2 ...
  // if level 3 ...
    switch (lvl) {
    case 1:
      setLimits(-250, 250);
      break;
    case 2:
      setLimits(-500, 500);
      break;
    case 3:
      setLimits(-750, 750);
      break;
    case 4:
      setLimits(-1000, 1000);
      break;
    case 5:
      setLimits(-1250, 1250);
      break;
    case 6:
      setLimits(-1500, 1500);
      break;
    case 7:
      setLimits(-1750, 1750);
      break;
    case 8:
      setLimits(-2000, 2000);
      break;
    case 9:
      setLimits(-2250, 2250);
      break;
  }
}

////set limits
//void setLimits(int ln, int lp) {
//  limitp = lp;
//  limitn = ln;
//  //AXIS 1 Limits
//  //108-109 is upper limit
//  int address = 108;
//  writeTwoRegisters(address, lp);
//  //110-111 is lower limit
//  address = 110;
//  writeTwoRegisters(address, ln);
//  //AXIS 2 Limits
//  //124-125 is upper limit
//  address = 124;
//  writeTwoRegisters(address, lp);
//  //126-127 is lower limit
//  address = 126;
//  writeTwoRegisters(address, ln);
//  enableActions(); // ACTIONS disable whenever a parameter is changed
//}

//set limits
void setLimits(int ln, int lp) {
  limitp = lp;
  limitn = ln;
  //34-35 is AXIS 1 upper limit
  //36-37 is AXIS 1 lower limit
  writeTwoRegisters(34, lp);
  writeTwoRegisters(36, ln);
  //38-39 is AXIS 2 upper limit
  //40-41 is AXIS 2 lower limit
  writeTwoRegisters(38, lp);
  writeTwoRegisters(40, ln);
}

void setVelLvl(int lvl) {
  // if level 1 use setVelocity for predefined value
  // if level 2 ...
  // if level 3 ...
  switch (lvl) {
    case 1:
      setVelocity(250);
      break;
    case 2:
      setVelocity(500);
      break;
    case 3:
      setVelocity(750);
      break;
    case 4:
      setVelocity(1000);
      break;
    case 5:
      setVelocity(1250);
      break;
    case 6:
      setVelocity(1500);
      break;
    case 7:
      setVelocity(1750);
      break;
    case 8:
      setVelocity(2000);
      break;
    case 9:
      setVelocity(2250);
      break;
  }
}

//void setVelocity(int vel) {
//  //set velocity in ACTIONS(task parameters)
//  //action 13 = neg, 14 = pos for AXIS 1 (address 112-113, 114-115)
//  //action 15 = neg, 16 = pos for AXIS 2 (address 128-129, 130-131)
//  int axis1neg = 112;
//  int axis1pos = 114;
//  int axis2neg = 128;
//  int axis2pos = 130;
//  writeTwoRegisters(axis1neg, -vel);
//  writeTwoRegisters(axis1pos, vel);
//  writeTwoRegisters(axis2neg, -vel);
//  writeTwoRegisters(axis2pos, vel);
//  //set general JOG velocities
//  //AXIS1.JOG.V is at 118 and 119
//  //AXIS1.JOG.V is at 120 and 121
//  int axis1 = 118;
//  int axis2 = 120;
//  writeTwoRegisters(axis1, vel);
//  writeTwoRegisters(axis2, vel);
//  enableActions();  // ACTIONS disable whenever a parameter is changed
//}

void setVelocity(int vel) {
  //set velocity for MOTION TASKS
  int MT2_a1 = 42;    // address 40042-40048 is velocity for motion task 2 & 3, for both axes
  int MT3_a1 = 44;
  int MT2_a2 = 46;
  int MT3_a2 = 48;
  writeTwoRegisters(MT2_a1, vel);
  writeTwoRegisters(MT3_a1, vel);
  writeTwoRegisters(MT2_a2, vel);
  writeTwoRegisters(MT3_a2, vel);
  //set general JOG velocities
  //AXIS1.JOG.V is at 118 and 119
  //AXIS1.JOG.V is at 120 and 121
  int axis1 = 118;
  int axis2 = 120;
  writeTwoRegisters(axis1, vel);
  writeTwoRegisters(axis2, vel);
}

void setComboVelLvl(int lvl) {
  // if level 1 use setVelocity for predefined value
  // if level 2 ...
  // if level 3 ...
  switch (lvl) {
    case 1:
      setComboVel(250);
      break;
    case 2:
      setComboVel(500);
      break;
    case 3:
      setComboVel(750);
      break;
    case 4:
      setComboVel(1000);
      break;
    case 5:
      setComboVel(1250);
      break;
    case 6:
      setComboVel(1500);
      break;
    case 7:
      setComboVel(1750);
      break;
    case 8:
      setComboVel(2000);
      break;
    case 9:
      setComboVel(2250);
      break;
  }
}

//void setComboVel(int vel) {
//  //set velocity in ACTIONS(task parameters)
//  //action 13 = neg, 14 = pos for AXIS 1 (address 112-113, 114-115)
//  //action 15 = neg, 16 = pos for AXIS 2 (address 128-129, 130-131)
//  int axis1neg = 112;
//  int axis1pos = 114;
//  int axis2neg = 128;
//  int axis2pos = 130;
//  writeTwoRegisters(axis1neg, -vel*0.75);
//  writeTwoRegisters(axis1pos, vel*0.75);
//  writeTwoRegisters(axis2neg, -vel*0.48);
//  writeTwoRegisters(axis2pos, vel*0.48);
//  //set general JOG velocities
//  //AXIS1.JOG.V is at 118 and 119
//  //AXIS1.JOG.V is at 120 and 121
//  int axis1 = 118;
//  int axis2 = 120;
//  writeTwoRegisters(axis1, vel*0.75);
//  writeTwoRegisters(axis2, vel*0.68);
//  enableActions();  // ACTIONS disable whenever a parameter is changed
//}

void setComboVel(int vel) {
  //set velocity for MOTION TASKS
  int MT2_a1 = 42;    // address 40042-40048 is velocity for motion task 2 & 3, for both axes
  int MT3_a1 = 44;
  int MT2_a2 = 46;
  int MT3_a2 = 48;
  writeTwoRegisters(MT2_a1, vel*0.75);
  writeTwoRegisters(MT3_a1, vel*0.75);
  writeTwoRegisters(MT2_a2, vel*0.48);
  writeTwoRegisters(MT3_a2, vel*0.48);
  //set general JOG velocities
  //AXIS1.JOG.V is at 118 and 119
  //AXIS1.JOG.V is at 120 and 121
  int axis1 = 118;
  int axis2 = 120;
  writeTwoRegisters(axis1, vel*0.75);
  writeTwoRegisters(axis2, vel*0.48);
}

void checkHomingMove() {
  if(incoming_data == axis1_down){
    jog(1,pos); // Axis 1 Down
  }
  else if(incoming_data == axis1_up){
    jog(1,neg); // Axis 1 Up
  }
  else if(incoming_data == axis2_down){
    jog(2,pos); // Axis 2 Down
  }
  else if(incoming_data == axis2_up){
    jog(2,neg); // Axis 2 Up
  }
  else if(incoming_data == both_up){
    jog(1,pos); // Both Up
    delay(50);
    jog(2,pos);
  }
  else if(incoming_data == both_down){
    jog(1,neg); // Both Down
    delay(50);
    jog(2,neg);
  }
  else if(incoming_data == both_stop){
    stopJog(); // Both Stop
  }
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

long actionRunning() {
  int address1 = 104;
  int address2 = 105;
  long data1 = modbusTCPClient.holdingRegisterRead(address1);
  long data2 = modbusTCPClient.holdingRegisterRead(address2);
  long result = (data1 << 16) + data2;
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

//void toggleMotion(dir a1, dir a2){
//  enableAxes();
//  delay(50);
//  jog(1,a1);
//  jog(2,a2);
//}

void toggleMotion(dir a1, dir a2){
  enableAxes();
  delay(50);
  int address = 30;
  if(a1 == pos){
    modbusTCPClient.holdingRegisterWrite(address - 1, 1);
    modbusTCPClient.holdingRegisterWrite(address - 1, 0);
  }
  if(a1 == neg){
    modbusTCPClient.holdingRegisterWrite(address + 1, 1);
    modbusTCPClient.holdingRegisterWrite(address + 1, 0);
  }
  if(a2 == pos){
    modbusTCPClient.holdingRegisterWrite(address, 1);
    modbusTCPClient.holdingRegisterWrite(address, 0);
  }
  if(a2 == neg){
    modbusTCPClient.holdingRegisterWrite(address + 2, 1);
    modbusTCPClient.holdingRegisterWrite(address + 2, 0);
  }
}

void continueMotion() {
  long pos1 = positionFeedback1();
  long pos2 = positionFeedback2();


}

//void go2zeroDegrees() {
//  int address = 137;      // for ACTION 23 & 24
//  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
//  modbusTCPClient.holdingRegisterWrite(address, 1);
//  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
//  modbusTCPClient.holdingRegisterWrite(address, 0);
//}

void go2zeroDegrees() {
  int address = 10;      // for MOTION TASK 0
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
  modbusTCPClient.holdingRegisterWrite(address, 0);
}

void setCurrentPosZeroAngle() {
  int address = 135;      // for ACTION 20 & 21
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
  modbusTCPClient.holdingRegisterWrite(address, 0);
}

void stopJog() {
  int address = 133;
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
  modbusTCPClient.holdingRegisterWrite(address, 0);
}

void disableLimits() {
  int address = 106;      // ACTION 13 & 14
  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
  modbusTCPClient.holdingRegisterWrite(address, 0);
  address = 122;          // ACTION 15 & 16
  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
  modbusTCPClient.holdingRegisterWrite(address, 0);
  address = 143;          // ACTION 25-28
  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
  modbusTCPClient.holdingRegisterWrite(address, 0);
  modbusTCPClient.holdingRegisterWrite(address + 1, 0);
  modbusTCPClient.holdingRegisterWrite(address + 2, 0);
}

void enableLimits() {
  int address = 106;      // ACTION 13 & 14
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
  address = 122;          // ACTION 15 & 16
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
  address = 143;          // ACTION 25-28
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address, 1);
  modbusTCPClient.holdingRegisterWrite(address + 1, 1);
  modbusTCPClient.holdingRegisterWrite(address + 2, 1);
}

long positionFeedback1() {
  int address = 139;
  long data1 = modbusTCPClient.holdingRegisterRead(address - 1);
  long data2 = modbusTCPClient.holdingRegisterRead(address);
  long result = (data1 << 16) + data2;
  return result/1000;
}

long positionFeedback2() {
  int address = 141;
  long data1 = modbusTCPClient.holdingRegisterRead(address - 1);
  long data2 = modbusTCPClient.holdingRegisterRead(address);
  long result = (data1 << 16) + data2;
  return result/1000;
}

void checkPos(){
  if (limitp - positionFeedback1() < 240)  {
    // axis 1 is close to positive limit, start negative
      dir axis1 = neg;
    }
    if (limitp - positionFeedback2() < 240)  {
      dir axis2 = neg;
    }
    if (limitn - positionFeedback1() > -240)  {
      dir axis1 = neg;
    }
    if (limitn - positionFeedback2() > -240)  {
      dir axis2 = neg;
    }
}

void clearModbusErr() {
  int address = 1;      // MODBUS.CLRERRORS
  modbusTCPClient.holdingRegisterWrite(address - 1, 1);
  modbusTCPClient.holdingRegisterWrite(address - 1, 0);
  modbusTCPClient.holdingRegisterWrite(address, 1);
  modbusTCPClient.holdingRegisterWrite(address, 0);
  modbusTCPClient.holdingRegisterWrite(address + 1, 1);
  modbusTCPClient.holdingRegisterWrite(address + 1, 0);
  modbusTCPClient.holdingRegisterWrite(address + 2, 1);
  modbusTCPClient.holdingRegisterWrite(address + 2, 0);
}
