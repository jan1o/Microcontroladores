#include <Ultrasonic.h>

#define pino_trigger 6
#define pino_echo 5

Ultrasonic ultrasonic(pino_trigger, pino_echo);

int objetos;

void setup() {
  // put your setup code here, to run once:
  objetos = 0;
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
}

void loop() {
  // put your main code here, to run repeatedly:
  float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  contador(cmMsec);
  
  Serial.print("Distancia em CM: ");
  Serial.println(cmMsec);
  
}

void contador(float distancia){
  if(distancia >= 1000 or distancia < 8){
    objetos += 1;
    }
    Serial.print("Números de objetos que passaram: ");
    Serial.println(objetos);
    delay(1000);
  }


