int latchPin = 4; //Pin 12 (RCLK) of the shift register to pin 4 on
                  //the Arduino – this will be referred to as the “latch pin”

int clockPin = 5; //Pin 11 (SRCLK) of the shift register to pin 5 on
                  //the Arduino – this will be referred to as the “clock pin”

int dataPin = 2;  //Pin 14 (SER) of the shift register to pin 2 on the
                  //Arduino – this will be referred to as the “data pin”

byte leds = 0;    //a variable called 'leds' is defined. This will be
                  //used to hold the pattern of which LEDs are currently turned on
                  //or off. Data of type 'byte' represents numbers using eight bits.
                  //Each bit can be either on or off, so this is perfect for keeping
                  //track of which of our eight LEDs are on or off.

int currentLED = 0;


void setup() {
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  leds = 0;

}

void loop() {
//  leds = 0;
//  if (currentLED == 7)
//    {
//    currentLED = 0;
//    }
//  else
//    {
//    currentLED++;
//    }
//  bitSet(leds, currentLED);
//  digitalWrite(latchPin, LOW);
//  shiftOut(dataPin, clockPin, LSBFIRST, leds);
//  digitalWrite(latchPin, HIGH);
//  delay(250);
    digitalWrite(latchPin, LOW);
    int b = 0b00000001;
    shiftOut(dataPin,clockPin, LSBFIRST, b);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    b = b<<1;
    shiftOut(dataPin,clockPin, LSBFIRST, b);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    b = b<<1;
    shiftOut(dataPin,clockPin, LSBFIRST, b);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    b = b<<1;
    shiftOut(dataPin,clockPin, LSBFIRST, b);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    b = b<<1;
    shiftOut(dataPin,clockPin, LSBFIRST, b);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    b = b<<1;
    shiftOut(dataPin,clockPin, LSBFIRST, b);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    b = b<<1;
    shiftOut(dataPin,clockPin, LSBFIRST, b);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
    b = b<<1;
    shiftOut(dataPin,clockPin, LSBFIRST, b);
    digitalWrite(latchPin, HIGH);
}
