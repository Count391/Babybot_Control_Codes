/* Read Tilt angles from Murata SCL3300 Inclinometer
 * Version 3.2.0 - September 3, 2021
 * Example2_BasicTiltReading
*/
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <SCL3300.h>

Servo s1;
Servo s2;
Servo s3;
Servo s4;
int v1=0;
int v2=0;
int M1=180;
int M2=0;
int M3 =180;
int M4 =0;
int x1 =0;
int home =0;

SCL3300 inclinometer;
//Default SPI chip/slave select pin is D10

// Need the following define for SAMD processors
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

void setup() {
  s1.attach(4);
  s2.attach(6);
  s3.attach(7);
  s4.attach(8);
  s1.write(180);
  s2.write(0);  
  s3.write(180);
  s4.write(0);


  Serial.begin(115200);
  delay(2000); //SAMD boards may need a long time to init SerialUSB
  Serial.println("Reading basic Tilt values from SCL3300 Inclinometer");

  if (inclinometer.begin() == false) {
    Serial.println("Murata SCL3300 inclinometer not connected.");
    while(1); //Freeze
  }
}

void loop() {
  
  if (inclinometer.available()) { //Get next block of data from sensor
    Serial.println();
    Serial.print("X Tilt: ");
    v1 = inclinometer.getTiltLevelOffsetAngleX();
    v2 = inclinometer.getTiltLevelOffsetAngleY();   
    Serial.print(v1);
    Serial.print("\t");
    Serial.print("Y Tilt: ");
    Serial.print(v2);
    Serial.println(); 
    delay(100); //Allow a little time to see the output

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
    

  else inclinometer.reset();
  }
}
