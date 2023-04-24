const int state=0;

const int  buttonPin = 1;    // the pin that the pushbutton is attached to
int previousState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 200;


void setup()
{
    pinMode(13, OUTPUT); // the three lights
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);

    pinMode(buttonPin, INPUT_PULLUP); //connect to internal resistance and 5V
}


void loop (){

    // Initial state
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);


    int reading = digitalRead(buttonPin);
    if (reading == HIGH && previousState == LOW && millis() - lastDebounceTime > debounceDelay)
    {

         digitalWrite(12, HIGH);
         delay(3000);
         digitalWrite(12, LOW);
 
         digitalWrite(11, HIGH);
         delay(3000);
         digitalWrite(11, LOW);

         digitalWrite(10, HIGH);
         delay(3000);
         digitalWrite(10, LOW);

     state++;
    }
