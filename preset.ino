#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin

#define driverPin1 5
#define driverPin2 6

#define LEFT_THRESHOLD  400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD    400
#define DOWN_THRESHOLD  800

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08


int xValue = 0 ; // To store value of the X axis
int yValue = 0 ; // To store value of the Y axis
int command = COMMAND_NO;

int leftPos = 127;     // Analog Write Vals
int rightPos = 127;     // Analog Write Vals
int oldLeft = 127;
int oldRight = 127;

void setup() {
  Serial.begin(115200) ;
  pinMode(4,OUTPUT);
  pinMode(3,INPUT);
  pinMode(driverPin1, OUTPUT);
  pinMode(driverPin2, OUTPUT);
  analogWrite(driverPin1, leftPos);
  analogWrite(driverPin2, rightPos);

}

void loop() {
  // read analog X and Y analog values
  


  // converts the analog value to commands
  // reset commands

  // NOTE: AT A TIME, THERE MAY BE NO COMMAND, ONE COMMAND OR TWO COMMANDS

  // print command to serial and process command
  digitalWrite(4,HIGH);
  int preset_trigger = digitalRead(3);
  if (preset_trigger) {
    Serial.println("COMMAND LEFT");
    rightPos = rightPos+1;
  }

  if (rightPos != oldRight) {
    Serial.print("Right Pos: ");
    Serial.println(rightPos);
  }
    analogWrite(driverPin2, rightPos);
    delay(50);
}
