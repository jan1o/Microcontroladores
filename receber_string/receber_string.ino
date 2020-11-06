#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#define SEC_REG 0x00
#define MIN_REG 0x01
/*byte readDS3231Register(byte regAddress){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(regAddress); // set DS3231 register pointer to regAddress
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 1); //requests 1 Byte from DS3231 register identified by regAddress
  return Wire.read();      
}

byte writeDS3231Register(byte regAddress, byte value){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(regAddress); // set DS3231 register pointer to regAddress
  Wire.write(value);
  Wire.write(0x01);
  Wire.endTransmission();
  //Wire.requestFrom(DS3231_I2C_ADDRESS, 1); //requests 1 Byte from DS3231 register identified by regAddress
  //Wire.write(value);      
}*/
byte dma;
void setup() {
  // put your setup code here, to run once:
 
  Wire.begin();
  Serial.begin(9600);
  //writeDS3231Register(SEC_REG,0X57);
}

void loop() {
  // put your main code here, to run repeatedly:
  dma = Stream.read();
  Serial.println(dma);
  delay(1000);
}
