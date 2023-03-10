// Assigning pins
int outputAngle = 5;
int outputSpeed = 6;
int enablePin = 7;
int startPin = 8;
int timeUpPin = 9;
int timeDownPin = 10;
int angle1Pin = 11;
int angle2Pin = 12;
int angle3Pin = 13;
int speed1Pin = A0;
int speed2Pin = A1;
int speed3Pin = A2;
int autoPin = A3;

// Initializing variables
int outputAngleValue = 0;
int outputSpeedValue = 0;
int time = 0;

// Setup function
void setup() {
  pinMode(outputAngle, OUTPUT);
  pinMode(outputSpeed, OUTPUT);
  pinMode(enablePin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(timeUpPin, INPUT_PULLUP);
  pinMode(timeDownPin, INPUT_PULLUP);
  pinMode(angle1Pin, INPUT_PULLUP);
  pinMode(angle2Pin, INPUT_PULLUP);
  pinMode(angle3Pin, INPUT_PULLUP);
  pinMode(speed1Pin, INPUT_PULLUP);
  pinMode(speed2Pin, INPUT_PULLUP);
  pinMode(speed3Pin, INPUT_PULLUP);
  pinMode(autoPin, INPUT_PULLUP);
}

// Loop function
void loop() {
  // Check if enable pin is 1
  if (digitalRead(enablePin) == 1) {
    // Check angle pins
    if (digitalRead(angle1Pin) == 1) {
      outputAngleValue = 1;
    } else if (digitalRead(angle2Pin) == 1) {
      outputAngleValue = 2;
    } else if (digitalRead(angle3Pin) == 1) {
      outputAngleValue = 3;
    }
    // Check speed pins
    if (digitalRead(speed1Pin) == 1) {
      outputSpeedValue = 1;
    } else if (digitalRead(speed2Pin) == 1) {
      outputSpeedValue = 2;
    } else if (digitalRead(speed3Pin) == 1) {
      outputSpeedValue = 3;
    }
    // Check timeUp pin
    if (digitalRead(timeUpPin) == 1) {
      if (time < 600) {
        time += 1;
      }
    }
    // Check timeDown pin
    if (digitalRead(timeDownPin) == 1) {
      if (time > 0) {
        time -= 1;
      }
    }
    // Check start pin
    if (digitalRead(startPin) == 1) {
      // Check if time is larger than 0
      if (time > 0) {
        // Check if auto pin is 1
        if (digitalRead(autoPin) == 1) {
          Serial.println("Output");
          // Check if enable pin is 1
          while (digitalRead(enablePin) == 1) {
            // Wait for enable pin to become 0
          }
        } else {
          Serial.println("Gamebar");
          // Check if enable pin is 1
          while (digitalRead(enablePin) == 1) {
            // Wait for enable pin to become 0
          }
        }
      }
    }
  }
}
