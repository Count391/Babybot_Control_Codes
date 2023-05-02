// Rotary Encoder Inputs
#define CLK 2
#define DT 3
#define SW 4

int RAN = 0;
int currentRAN;
int lastRAN;
String currentRANDir ="";
unsigned long lastRANButtonPress = 0;

void setup() {
        
  // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of CLK
  lastRAN = digitalRead(CLK);
}

void loop() {
        
  // Read the current state of CLK
  currentRAN = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentRAN != lastRAN  && currentRAN == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentRAN) {
      RAN --;
      currentRANDir ="CW";
    } else {
      // Encoder is rotating CW so increment
      RAN ++;
      currentRANDir ="CCW";
    }

    Serial.print("Direction: ");
    Serial.print(currentRANDir);
    Serial.print(" | RAN: ");
    Serial.println(RAN);
  }

  // Remember last CLK state
  lastRAN = currentRAN;

  // Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastRANButtonPress > 50) {
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastRANButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
}
