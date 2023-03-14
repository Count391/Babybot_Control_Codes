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
const int BUTTON1 = 7;
const int BUTTON2 = 6;
int buttonState1 = 0;
int buttonState2 = 0;


void setup() {
  //Buttons setup
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
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
      printReg(96,131);
    }
  } else {
    
  }

  //Button Loop
  int prevButtonState1 = buttonState1;
  buttonState1 = digitalRead(BUTTON1);
  int prevButtonState2 = buttonState2;
  buttonState2 = digitalRead(BUTTON2);
  if(buttonState1 != prevButtonState1){
    Serial.print("Button 1: ");
    Serial.println(buttonState1);
    delay(500);
  //limits(900, 600);
  //printReg(108,111);
  //printReg(124,127);
  }
  if(buttonState2 != prevButtonState2){
    Serial.print("Button 2: ");
    Serial.println(buttonState2);
    delay(500);
    jog();
  }

}

String printReg(int a, int b) {
    for(int i = a; i<b; i++){
          uint16_t Register = modbusTCPClient.holdingRegisterRead(i);
          Serial.print("Register ");
          Serial.print(40001+i);
          Serial.print(": ");
          Serial.println(Register,HEX);
          //Serial.println(Register << 16, HEX);
          int a = Register << 8;
          Serial.println(a, HEX);
          a = a >> 8;
          Serial.println(a, HEX);
      }
}

void limits(int lp, int ln) {
  int lu1 = lp >> 16;
  int lu2 = lp << 16;
  lu2 = lu2 >> 16;
  int ll1 = ln >> 16;
  int ll2 = lp << 16;
  ll2 = ll2 >> 16;
  //AXIS 1 Limits
  //108-109 is upper limit
  int address1 = 108;
  int address2 = 109;
  modbusTCPClient.holdingRegisterWrite(address1-1,lu1);
  modbusTCPClient.holdingRegisterWrite(address2-1,lu2);
  //110-111 is lower limit
  modbusTCPClient.holdingRegisterWrite(address1-1,ll1);
  modbusTCPClient.holdingRegisterWrite(address2-1,ll2);
  //AXIS 2 Limits
  //124-125 is upper limit
  modbusTCPClient.holdingRegisterWrite(address1-1,lu1);
  modbusTCPClient.holdingRegisterWrite(address2-1,lu2);
  //126-127 is lower limit
  modbusTCPClient.holdingRegisterWrite(address1-1,ll1);
  modbusTCPClient.holdingRegisterWrite(address2-1,ll2);
}

void enableAxes(int address) {
  
}

void jog() {
  //Enable and disable
  int address1 = 100;
  int address2 = 102;
  modbusTCPClient.holdingRegisterWrite(address1-1,1);
  modbusTCPClient.holdingRegisterWrite(address2-1,1);
  modbusTCPClient.holdingRegisterWrite(address1-1,0);
  modbusTCPClient.holdingRegisterWrite(address2-1,0);
}

int currentData(){
  
}
