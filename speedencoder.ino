// Rotary Encoder Inputs
#define CLK 2
#define DT 3
#define SW 4

int spd = 0;
int currentSPD;
int lastSPD;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {
        
  // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of CLK
  lastSPD = digitalRead(CLK);
}

void loop() {
        
  // Read the current state of CLK
  currentSPD = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentSPD != lastSPD  && currentSPD == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentSPD) {
      spd --;
      currentDir ="CW";
    } else {
      // Encoder is rotating CW so increment
      spd ++;
      currentDir ="CCW";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | spd: ");
    Serial.println(spd);
  }

  // Remember last CLK state
  lastSPD = currentSPD;

  // Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
}
