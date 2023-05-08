#include <ezButton.h>

ezButton toggleSwitch(4);  // create ezButton object that attach to pin 7;

void setup() {
  Serial.begin(9600);
  toggleSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  toggleSwitch.loop(); // MUST call the loop() function first

  if (toggleSwitch.isPressed())
    Serial.println("The switch: OFF -> ON");

  if (toggleSwitch.isReleased())
    Serial.println("The switch: ON -> OFF");

  int state = toggleSwitch.getState();
  if (state == HIGH)
    Serial.println("The switch: OFF");
  else
    Serial.println("The switch: ON");
}
