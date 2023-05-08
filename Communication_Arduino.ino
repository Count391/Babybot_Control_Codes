//for Mega
char mystr[10]; //Initialized variable to store recieved data


void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  while(!Serial2.available()){
    
  }
  Serial2.readBytes(storedVariable,10);
  Serial.println(storedVariable); //Print data on Serial Monitor
}
