//for Mega with Control Panel

int spd;
int angl;
int bounce;
int axis;
int checked = 0;
int incoming_data;
int previous_data;

void checkValues(int message) {
  spd = message%10;
  message = message/10;
  angl = message%10;
  message = message/10;
  bounce = message%10;
  message = message/10;
  axis = message%10;
}

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  while(!Serial3.available()){
    
  }
  incoming_data = Serial3.read();
  if (incoming_data/10 == 1){
    spd = incoming_data%10;
  }
  if (incoming_data/10 == 2){
    angl = incoming_data%10;
  }
  if (incoming_data/10 == 3){
    axis = incoming_data%10;
  }
  if (incoming_data/10 == 4){
    bounce = incoming_data%10;
  }
  //Serial.println(incoming_data);
  if(incoming_data == 100){
  Serial.print("Speed is at: ");
  Serial.println(spd);
  Serial.print("Angle is at: ");
  Serial.println(angl);
  Serial.print("Axis is at: ");
  Serial.println(axis);
  Serial.print("Bounce is at: ");
  Serial.println(bounce);
  }
}
