#include <Event.h>
#include <Timer.h>
#include <Ultrasonic.h>

//definições de pinos de outros
#define trigger 8        //trigger ultrassom
#define echo 9           //echo ultrassom
#define IN1 5            //pinos Arduino ligados a entrada da Ponte H
#define IN2 4            //pinos Arduino ligados a entrada da Ponte H
#define START 3          //liga a esteira quando o braço termina
#define BRACO 11          //diz ao braço que pode começar
#define LDR_A 13         //entrada analogica do ldr

//criação de objetos
Timer t;
Ultrasonic ultrasonic(trigger, echo);

//funções
void conta_objetos();             //mede a distancia e diz se está passando algo
void laser();                     //mapeamento de luz
void SerialPrint();               //para ver no monitor serial
void tamanho(bool laser_down);    //mostra o tamanho do objeto em P, M, G
void ligaMotor();                 //faz o motor funcionar em sentido horario
void paraMotor();                 //faz o motor parar

//variáveis globais
bool flag_SR04 = true;            //sinaliza para a função conta_objetos
bool flag_LDR = false;            //sinaliza para a função lazer
bool flag_Esteira = false;        //sinaliza para ativar ou desativar o motor
int objetos = 0;                  //conta a quatidade de objetos que passou
int distancia;                     //auxilia na contagem dos objetos
int resistance;                   //auxilia na verificação do tamanho
int dimensao;                     //retorna o tamanho em pulsos de 10 milissegundos
int dimensaoAux;                  //auxilia na condição de parada da esteira

//potenciometro
int pot = 0;
int pulseWidth;

unsigned long startMillis;        //captura o tempo quando um objeto termina de passar
unsigned long currentMillis;      //atualiza o tempo presente na velocidade máxima do loop

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(LDR_A, INPUT);
  startMillis = millis();
  ligaMotor();
  attachInterrupt(digitalPinToInterrupt(START), ligaMotor, RISING);
  
  //início de funções
  t.every(10, laser);
  t.every(50, conta_objetos);
  t.every(1000, SerialPrint);
}

void loop() {
  currentMillis = millis();
  pot = analogRead(A0);
  pulseWidth = map(pot, 0, 1023, 0, 255);
  if (flag_Esteira and ((currentMillis - startMillis) > dimensaoAux)) {
    paraMotor();
    digitalWrite(BRACO, HIGH);
    flag_Esteira=!flag_Esteira;
    digitalWrite(BRACO, LOW);
   }
  t.update();
}

void conta_objetos() {
  distancia = ultrasonic.distanceRead();
  if ((distancia >= 1000 or distancia < 8) and flag_SR04) {
    flag_SR04 = false;
    objetos ++;
  }
  else if ((!(distancia >= 1000 or distancia < 8)) and (!flag_SR04)) {
    flag_SR04 = true;
  }
}

void SerialPrint() {
  Serial.print("Números de objetos que passaram: ");
  Serial.println(objetos);
  Serial.print("Distancia em CM: ");
  Serial.println(distancia);
  Serial.print("Resistência: ");
  Serial.println(resistance);
  Serial.print("Dimensão em pulsos de centissegundos: ");
  Serial.println(dimensao);
  Serial.print("dimensaoAux: ");
  Serial.print(dimensaoAux);
  Serial.print("    IN1: ");
  Serial.print(digitalRead(IN1));
  Serial.print("    IN2: ");
  Serial.print(digitalRead(IN2));
  Serial.print("    BRACO: ");
  Serial.println(digitalRead(BRACO));
  Serial.print("Tamanho: ");
  Serial.println(dimensao);
  Serial.print("\n");
}
void laser() {
  //map(pino A1, luz baixa, luz alta, resistência alta, resistência baixa)
  //quanto mais luz, menor a resistência
  //resistance = map(analogRead(LDR_A), 0, 1023, 0, 255);
  if (digitalRead(LDR_A) == 1) {   //se está passando um objeto o laser cai e aumenta a tensão,
    flag_LDR = true;      //para começar a contar colocamos true na flag_LDR
  }
  else {
    flag_LDR = false;
  }
  tamanho(flag_LDR);
}

void tamanho(bool laser_down) {
  if (digitalRead(LDR_A) == 1 and laser_down) {
    dimensao ++;
    flag_Esteira = true;
  }
  else {
    if (dimensao > 0) {         //caso a resistência caia é sinal que passou um objeto.
      startMillis = millis();   //capturamos o momento e
      dimensaoAux = dimensao;   //a dimensão para usar para controlar o motor e bracinho
    }
    dimensao = 0;               //para evitar problemas colocamos dimensão em 0 logo
  }
}

void ligaMotor() {
  analogWrite(IN1, pulseWidth);
  //digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void paraMotor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
}







