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
  pinMode(driverPin1, OUTPUT);
  pinMode(driverPin2, OUTPUT);
  analogWrite(driverPin1, leftPos);
  analogWrite(driverPin2, rightPos);
}

void loop() {
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  oldLeft = leftPos;
  oldRight = rightPos;

  // converts the analog value to commands
  // reset commands
  command = COMMAND_NO;

  // check left/right commands
  if (xValue < LEFT_THRESHOLD)
    command = command | COMMAND_LEFT;
  else if (xValue > RIGHT_THRESHOLD)
    command = command | COMMAND_RIGHT;

  // check up/down commands
  if (yValue < UP_THRESHOLD)
    command = command | COMMAND_UP;
  else if (yValue > DOWN_THRESHOLD)
    command = command | COMMAND_DOWN;

  // NOTE: AT A TIME, THERE MAY BE NO COMMAND, ONE COMMAND OR TWO COMMANDS

  // print command to serial and process command
  if (command & COMMAND_LEFT) {
    Serial.println("COMMAND LEFT");
    leftPos = leftPos+1;
  }

  if (command & COMMAND_RIGHT) {
    Serial.println("COMMAND RIGHT");
    leftPos = leftPos-1;
  }

  if (command & COMMAND_UP) {
    Serial.println("COMMAND UP");
    rightPos = rightPos+1;
  }

  if (command & COMMAND_DOWN) {
    Serial.println("COMMAND DOWN");
    rightPos = rightPos-1;
  }
  if (leftPos != oldLeft) {
    Serial.print("Left Pos: ");
    Serial.println(leftPos);
  }
  if (rightPos != oldRight) {
    Serial.print("Right Pos: ");
    Serial.println(rightPos);
  }
    analogWrite(driverPin1, leftPos);
    analogWrite(driverPin2, rightPos);
    delay(50);
}
