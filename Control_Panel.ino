#include <Encoder.h> // Library for the rotary encoder

// Initialize the rotary encoder with its 2 pins
Encoder myEnc(2, 3); 

int enablePin = 22; // Pin for the "enable" variable
int pausePin = 23; // Pin for the "pause" variable
int time = 0; // Variable to store the time value
int intensity = 0; // Variable to store the intensity value
bool enable = 0; // Variable to store the state of "enable" (1 = enabled, 0 = disabled)
bool pause = 0; // Variable to store the state of "pause" (1 = paused, 0 = not paused)

void setup() {
  pinMode(enablePin, INPUT_PULLUP); // Set the "enable" pin as an input with a pull-up resistor
  pinMode(pausePin, INPUT_PULLUP); // Set the "pause" pin as an input with a pull-up resistor
}

void loop() {
  // Read the state of the "enable" variable from pin 22
  int enableState = digitalRead(enablePin);
  // If the button is pressed and the state of "enable" is changing, toggle the "enable" variable
  if (enableState == LOW && enable == 1) {
    enable = 0;
  }
  else if (enableState == LOW && enable == 0) {
    enable = 1;
  }

  // Read the state of the "pause" variable from pin 23
  int pauseState = digitalRead(pausePin);
  // If the button is pressed and the state of "pause" is changing, toggle the "pause" variable
  if (pauseState == LOW && pause == 1) {
    pause = 0;
  }
  else if (pauseState == LOW && pause == 0) {
    pause = 1;
  }

  // If the "enable" variable is set to 1, process the input from the rotary encoder
  if (enable == 1) {
    // Read the position of the rotary encoder
    int encoderValue = myEnc.read();
    // If the encoder has been turned, update the "time" variable
    if (encoderValue != 0) {
      time += encoderValue;
    }

    // Check if the rotary encoder has been pushed
    if (digitalRead(4) == LOW) {
      // If pushed, set "intensity" to the current value of "time"
      intensity = time;
      // Limit the value of "intensity" to 5
      if (intensity > 5) {
        intensity = 5;
      }
    }
  }

  // If the "enable" variable is set to 0, do not take any input from the user except the "enable" pin
  else {
    // Reset the values of "time" and "intensity" to 0
    time = 0;
    intensity = 0;
  }
}