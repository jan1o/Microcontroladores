#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define DS3231_I2C_ADDRESS 0x68
#define SEC_REG 0x00
#define MIN_REG 0x01
#define HOU_REG 0x02
#define DAY_REG 0x03
#define DATE_REG 0x04
#define MON_REG 0x05
#define YEA_REG 0x06
#define TEMP_REG1 0x11
#define TEMP_REG2 0x12

#define A1MASC1 0x07
#define A1MASC2 0x08
#define A1MASC3 0x09
#define A1MASC4 0x0A

int i = 1;

byte _sec, sec_; //segundos
byte _min, min_; //minutos
byte _hou, hou_; String AM_PM_24; //hora
byte _date, date_; //dia do mês 
byte _mon, mon_; //mês
byte _yeaDU, yeaDU_; //ano parte da dezena e unidade

byte _sec_ = 0;
byte _min_ = 0;
byte _hou_ = 0;
byte _day_ = 0; //dia da semana
byte _date_ = 0; 
byte _mon_ = 0; 
byte _yeaDU_ = 0;

byte temperaturaInt; 
byte temperaturaDec;

String dayVector[] = {"Dom","Seg", "Ter","Qua","Qui","Sex","Sab"}; //vetor de dias da semana em string

//protótipo de funções
byte readDS3231Register(byte regAddress);
byte writeDS3231Register(byte regAddress, byte value);
void secDec(byte leu_Reg);
void minDec(byte leu_Reg);
void houDec(byte leu_Reg);
void dateDec(byte leu_Reg); 
void monDec(byte leu_Reg); 
void yeaDec(byte leu_Reg);
void temperaturaInteira(byte leu_Reg);
void temperaturaDecimal(byte leu_Reg); 

//LiquidCrystal lcd(12,11,5,4,3,2);
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);
void setup() {
  lcd.begin(16,2);
  //pinMode(9,OUTPUT);
  //analogWrite(9,96);
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  writeDS3231Register(SEC_REG, 0X56);
  writeDS3231Register(MIN_REG, 0x59);
  writeDS3231Register(HOU_REG, B00100011);
  //24h 18:59:55  B00011000 ou PM 08:59:55 B01101000 ou AM 08:59:55 B01001000
  //24h B0010 0011(23:mm:ss) ou PM B0111 0001(11:mm:ss) AM B0101 0001(11:mm:ss)
  writeDS3231Register(DAY_REG, 0x07); 
  writeDS3231Register(DATE_REG, 0x07);
  writeDS3231Register(MON_REG, 0x04); 
  writeDS3231Register(YEA_REG, 0x18);
}

void loop() {
  // put your main code here, to run repeatedly:
  _sec_ = readDS3231Register(SEC_REG);
  _min_ = readDS3231Register(MIN_REG);
  _hou_ = readDS3231Register(HOU_REG);
  _day_ = readDS3231Register(DAY_REG);
  _date_ = readDS3231Register(DATE_REG); 
  _mon_ = readDS3231Register(MON_REG);
  _yeaDU_ = readDS3231Register(YEA_REG);
  temperaturaInt = readDS3231Register(TEMP_REG1);
  temperaturaDec = readDS3231Register(TEMP_REG2);
  
  secDec(_sec_);
  minDec(_min_);
  houDec(_hou_);
  dateDec(_date_); 
  monDec(_mon_); 
  yeaDec(_yeaDU_);

  lcd.clear();
  lcd.setCursor(1,0); 
    if(i<=30){
    lcd.print(dayVector[_day_ - 1]);  
    lcd.print(" ");
    lcd.print(date_, DEC);
    lcd.print(_date, DEC);
    lcd.print("/");
    lcd.print(mon_, DEC);
    lcd.print(_mon, DEC);
    lcd.print("/");
    lcd.print(20);
    lcd.print(yeaDU_, DEC);
    lcd.print(_yeaDU, DEC);
    i++;    
    }else{
    if(i>=35){
      i=1;
    }
    temperaturaInteira(temperaturaInt);
    temperaturaDecimal(temperaturaDec);
    i++;
  }

  lcd.setCursor(1,1);
  lcd.print(AM_PM_24);
  lcd.print(" ");
  lcd.print(hou_, DEC);
  lcd.print(_hou, DEC);
  lcd.print(":");
  lcd.print(min_, DEC);
  lcd.print(_min, DEC);
  lcd.print(":");
  lcd.print(sec_, DEC);
  lcd.print(_sec, DEC);
  delay(1000);
  
  

}

byte readDS3231Register(byte regAddress) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(regAddress); // set DS3231 register pointer to regAddress
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 1); //requests 1 Byte from DS3231 register identified by regAddress
  return Wire.read();
}

byte writeDS3231Register(byte regAddress, byte value) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(regAddress); // set DS3231 register pointer to regAddress
  Wire.write(value);
  Wire.write(0x01);
  Wire.endTransmission();
}

void secDec(byte leu_Reg) {
  _sec = leu_Reg << 4;
  _sec = _sec >> 4;
  sec_ = leu_Reg >> 4;
}

void minDec(byte leu_Reg) {
  _min = leu_Reg << 4;
  _min = _min >> 4;
  min_ = leu_Reg >> 4;
}

void houDec(byte leu_Reg) {
  _hou = leu_Reg << 4;
  _hou = _hou >> 4;//bits referente às unidades de minutos
  if(leu_Reg >> 6 == B00000000){//01hhxxxx 24h
    hou_ = leu_Reg << 2;
    hou_ = hou_ >> 6;
    AM_PM_24 = "24";
  } else{//01chxxxx AM/PM
    byte teste = leu_Reg << 2;
    hou_ = teste << 1;
    hou_ = hou_ >> 7;
    if(teste >> 7 == B00000001)
      AM_PM_24 = "PM";
    else
      AM_PM_24 = "AM";
  }
}

// nao é necessaria uma dayDec porque a hr so precisa dos 3 primeiros bits

void dateDec(byte leu_Reg){ 
  _date = leu_Reg << 4;
  _date = _date >> 4;
  date_ = leu_Reg >> 4;
}

void monDec(byte leu_Reg){ 
  _mon = leu_Reg << 4;
  _mon = _mon >> 4;
  mon_ = leu_Reg >> 4; 
}

void yeaDec(byte leu_Reg){
  _yeaDU = leu_Reg << 4;
  _yeaDU = _yeaDU >> 4;
  yeaDU_ = leu_Reg >> 4;
}

  void temperaturaInteira(byte leu_Reg){
  byte temp;
  if(leu_Reg >> 7 == B00000001){
    temp = (leu_Reg ^ B11111111) + 1;
    lcd.print("-");
    lcd.print(temp, DEC);
  }
  else{
    temp = leu_Reg;
    lcd.print("+");
    lcd.print(temp, DEC);
  }
}

void temperaturaDecimal(byte leu_Reg) {
  lcd.print(".");
  if(leu_Reg >> 6 == 0x00){
    lcd.print("00");
  }
  else if(leu_Reg >> 6 == 0x01){
    lcd.print("25");
  }
  else if(leu_Reg >> 6 == 0x02){
    lcd.print("50");
  }
  else{
    lcd.print("75");
  }
  
}


