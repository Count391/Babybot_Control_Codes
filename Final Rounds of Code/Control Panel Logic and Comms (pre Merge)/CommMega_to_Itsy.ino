//for Mega with Itsy

int my_data = 45;
int my_data2 = 25;
int incoming_data;

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  while(!Serial3.available()){
    
  }
  incoming_data = Serial3.read();
  if(incoming_data == my_data){
    Serial.println("Axis 1 Down");
    Serial.println(incoming_data);
  }
  else if(incoming_data == my_data2){
    Serial.println("Axis 1 Up");
    Serial.println(incoming_data);
  }
}
