int state=0;
int lightPin = 0;
const int  buttonPin = 1;    // the pin that the pushbutton is attached to
const int  LED1 = 13;
const int  LED2 = 14;
const int  LED3 = 15;
int previousState = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 200;


void setup()
{
    pinMode(A1, OUTPUT); // the three lights
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);

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

     state++;
     Serial.print("Current Mode: ");
     Serial.print(state);

     lightPin = 12 + state;
     digitalWrite(13, LOW);
     digitalWrite(14, LOW);    
     digitalWrite(15, LOW);
     digitalWrite(lightPin, HIGH); 
     
    }
    if (state == 4){
      state = 0;
    }
}
