int readByte(Stream &port) {
    int readByte = port.read(); // read next available byte
    return readByte;
}
void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  //Serial3.begin(9600);
}

void loop(){
  Serial.print("Reading from Serial Port 1: ");
  int port1 = readByte(Serial1);
  Serial.println(port1);
  Serial2.write(port1);
  Serial.print("Reading from Serial Port 2: ");
  int port2 = readByte(Serial2);
  Serial.println(port2);
  //Serial.print("Reading from Serial Port 3: ");
  //int port3 = readByte(Serial3);
  //Serial.println(port3);
}
