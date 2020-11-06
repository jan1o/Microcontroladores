#include <Wire.h>
#define second 0x00
#define minute 0x01
#define rtcds3231 0x68


byte RTCDS3231(byte regAdress, byte value){
  Wire.beginTransmission(rtcds3231);
  Wire.write(regAdress);
  Wire.write(value);
  wire.endTransmission();
}
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
 Serial.begin(9600);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
