void setup() {
  int pinNumber = 22;
  while(pinNumber <= 26){
    pinMode(pinNumber, INPUT_PULLUP); 
    pinNumber++;
  }                                             //Set the input pins
  Serial.begin(96000);
}

void loop() {
  int A = digitalRead(22);
  Serial.println(A);
  delay(500);
}
