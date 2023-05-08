

/* Read Tilt angles from Murata SCL3300 Inclinometer
 * Version 3.2.0 - September 3, 2021
 * Example2_BasicTiltReading
*/
#include <Wire.h>
#include <SPI.h>
#include <SCL3300.h>

double roll = 0;
double pitch =0;
int x1 =0;
int home =0;

SCL3300 inclinometer;
//Default SPI chip/slave select pin is D10

// Need the following define for SAMD processors
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  #define Serial SERIAL_PORT_USBVIRTUAL
  #endif

  void homingSystem (){
  int homingStatus = 0;  
  if(inclinometer.available()) { //Get next block of data from sensor
    Serial.println();
    Serial.print("Roll Tilt: ");
    pitch = inclinometer.getTiltLevelOffsetAngleX();  // pitch value is saved
    Serial.print(roll);
    Serial.print("\t");
    Serial.print("Pitch Tilt is : ");   
    roll = inclinometer.getTiltLevelOffsetAngleY();   // roll value is saved
    Serial.print(pitch);
    Serial.print("\t"); 
    Serial.println(); 
    delay(400); //Allow a little time to see the output
      // Got all the ROll and Pitch values 
     // Now we starts Homing logic based on those values
    while(homingStatus != 1){
      while (roll < 0){             // this means Axis 1 is higher and I set 0.1 for okay error  
          
        if(inclinometer.available()) { //Get next block of data from sensor
          Serial.println();
          Serial.print("Roll Tilt: ");
          roll = inclinometer.getTiltLevelOffsetAngleY();  // pitch value is saved
        } 
        else {
          inclinometer.reset();
        } 
              
        Serial.println("roll <0 ");
        Serial.println(roll);        
        delay(800);      }
        
      while (roll > 0){             
        roll = inclinometer.getTiltLevelOffsetAngleY();
        Serial.println("roll >0 "); 
        Serial.println(roll); 
        delay(800);
      // move the Axis 1 to go up some amount
      } // at this point the roll is about 0(axis 1 and 2 have same height) so and we only need to adjust same
      //amount of pitch for both axes(height to be 0 = plat)
      while (pitch > 0) {
        pitch = inclinometer.getTiltLevelOffsetAngleX();
        Serial.println("pitch >0 ");
        Serial.println(pitch);
        delay(800);
      // move Axis 1 & 2 to go down some amount
      }
      while (pitch < 0) {
      // move Axis 1 & 2 to go up some amount
      pitch = inclinometer.getTiltLevelOffsetAngleX();
      Serial.println("pitch <0 ");
      Serial.println(pitch);
      delay(800);
      }
      if (abs(roll) <0.5 && abs(pitch) <0.5){   // after finishing adjustment, check if the platform is at home 0.5 = acceptable error range
        homingStatus = 1;
      }
    }
  }
  else {
    inclinometer.reset();
  }
  }


  

void setup() {
  
  Serial.begin(9600);
  delay(2000); //SAMD boards may need a long time to init SerialUSB
  Serial.println("Reading basic Tilt values from SCL3300 Inclinometer");

  if (inclinometer.begin() == false) {
    Serial.println("Murata SCL3300 inclinometer not connected.");
    while(1); //Freeze
    homingSystem();
  }
  

}

void loop() {
homingSystem();
delay(1000);
}
  
/*
    if ((v2 > 0) && (x1 == 0) && (home==0)){
      Serial.print("Y-axis zeroing by moving negative direction");
      Serial.println();
      M2 = M2 + 1;
      s2.write(M2);
      }
    else if ((v2 < 0) && (x1 ==0) && (home==0) ){
      Serial.print("Y-axis zeroing by moving positive direction");
      Serial.println();
      M1 = M1 - 1;
      s1.write(M1);
      }
/*    
    else if ((-1 <= v2 <= 1) && (x1 ==0) &&(home==0)) {
      Serial.print("Y-axis zeroed, starting X-axis zeroing");
      Serial.println();
      x1 = 1;
      s2.write(0);
      s1.write(180);
    }

    else if((v1 > 0) && (x1 == 1) &&(home==0)){
      Serial.print("X-axis zeroing by moving negative direction");
      Serial.println();
      M3 = M3 - 1;
      s3.write(M3);
      }
    else if ((v1 < 0) && (x1 ==1) &&(home==0)){
      Serial.print("X-axis zeroing by moving positive direction");
      Serial.println();
      M4 = M4 + 1;
      s4.write(M4);
      }
    else if((-1<v1<1) && (x1==1) && (-1<v2<1) &&(home==0)){
      Serial.print("The homing is done");
      Serial.println();
      s3.write(180);
      s4.write(0);
      x1 = 1;
    }
    else if( (v1<-1) ||(v1>1) || (v2<-1)|| (v2>1) && (x1 ==1) &&(home==0)){
      x1 =0;
      s3.write(180);
      s4.write(0);
    }
    else if( (-1<v1<1) && (-1<v2<1) &&(home==0)){
    s1.write(180);
    s2.write(0);  
    s3.write(180);
    s4.write(0);
    home =1;
    }  
    
*/
