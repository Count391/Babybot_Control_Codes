//HT16K33 - Displaying Alphanumeric Characters
//------------------------------------------------

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include <Wire.h>
//-------------------------------------------------
uint16_t number[] = {0x0C3F,0x0406,0x00DB,0x008F,0x00E6,
                   /* 5      6      7      8      9 */
                     0x00ED,0x00FD,0x01401,0x00FF,0x00EF};
                   /* A      B      C      D      E      F      G */
uint16_t letter[] = {0x00F7,0x128F,0x0039,0x120F,0x00F9,0x00F1,0x00BD,
                   /* H      I      J      K      L      M      N */
                     0x00F6,0x1209,0x001E,0x2470,0x0038,0x0536,0x2136,
                   /* O      P      Q      R      S      T      U */
                     0x003F,0x00F3,0x203F,0x20F3,0x018D,0x1201,0x003E,
                   /* V      W      X      Y      Z */
                     0x0C30,0x2836,0x2D00,0x1500,0x0C09};
//======================================================================
void setup()
{
  Wire.begin();
  //------------------
  HT16K33_init(0x70);
  HT16K33_init(0x71);
  //------------------
  disp_clear(0x70);
  disp_clear(0x71);
  //------------------
  delay(500);
}
//======================================================================
void loop()
{
  disp_numbers();                 //display numbers from 0 to 9
  disp_letters();                 //display letters from A to Z
}
//======================================================================
void HT16K33_init(byte address)
{  
  Wire.beginTransmission(address);//send address of HT16K33
  Wire.write(0x21);               //normal mode
  Wire.endTransmission(false);
  //---------------------------------------------------------------
  Wire.beginTransmission(address);
  Wire.write(0xE8);               //brightness level (0xE0 to 0xEF)
  Wire.endTransmission(false);
  //---------------------------------------------------------------
  Wire.beginTransmission(address);
  Wire.write(0x81);               //display ON, blinking OFF
  Wire.endTransmission();
}
//======================================================================
void disp_alphaNum(byte add, byte digit_add, uint16_t character)
{
  Wire.beginTransmission(add);
  Wire.write(digit_add);                //send digit address
  Wire.write(character & 0x00FF);       //send low byte of 16-bit char
  Wire.write((character & 0xFF00) >> 8);//send high byte of 16-bit char
  Wire.endTransmission();
}
//======================================================================
void disp_clear(byte add)
{
  for(byte i=0; i<=3; i++) disp_alphaNum(add, i*2, 0);
}
//======================================================================
void disp_numbers()
{
  byte j = 0;
  for(byte i=0; i<=3; i++)
  {
    disp_alphaNum(0x70, j, number[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  j = 0;
  for(byte i=4; i<=7; i++)
  {
    disp_alphaNum(0x71, j, number[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  delay(200);
  disp_clear(0x70); disp_clear(0x71);
  delay(200);
  //----------------------------------
  j = 0;
  for(byte i=8; i<=9; i++)
  {
    disp_alphaNum(0x70, j, number[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  delay(500);
  disp_clear(0x70); disp_clear(0x71);
  delay(500);
}
//======================================================================
void disp_letters()
{
  byte j = 0;
  for(byte i=0; i<=3; i++)
  {
    disp_alphaNum(0x70, j, letter[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  j = 0;
  for(byte i=4; i<=7; i++)
  {
    disp_alphaNum(0x71, j, letter[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  delay(200);
  disp_clear(0x70); disp_clear(0x71);
  delay(200);
  //----------------------------------
  j = 0;
  for(byte i=8; i<=11; i++)
  {
    disp_alphaNum(0x70, j, letter[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  j = 0;
  for(byte i=12; i<=15; i++)
  {
    disp_alphaNum(0x71, j, letter[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  delay(500);
  disp_clear(0x70); disp_clear(0x71);
  delay(500);
  //----------------------------------
  j = 0;
  for(byte i=16; i<=19; i++)
  {
    disp_alphaNum(0x70, j, letter[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  j = 0;
  for(byte i=20; i<=23; i++)
  {
    disp_alphaNum(0x71, j, letter[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  delay(500);
  disp_clear(0x70); disp_clear(0x71);
  delay(500);
  //----------------------------------
  j = 0;
  for(byte i=24; i<=25; i++)
  {
    disp_alphaNum(0x70, j, letter[i]);
    delay(200);
    j += 2;
  }
  //----------------------------------
  delay(500);
  disp_clear(0x70); disp_clear(0x71);
  delay(500);
}
