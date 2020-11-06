//Programa : Controle 2 motores DC usando Ponte H L298N

//Definicoes pinos Arduino ligados a entrada da Ponte H
int IN1 = 4;
int IN2 = 5;
int IN3 = 6;
int IN4 = 7;
int pot = 0;
int pulseWidth;
 
void setup()
{
 //Define os pinos como saida
 pinMode(IN1, OUTPUT);
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);
}
 
void loop()
{
 pot = analogRead(0);
 pulseWidth = map(pot, 0, 1023, 0, 255);
 analogWrite(IN1, pulseWidth);
 digitalWrite(IN2, LOW);
 analogWrite(IN3, pulseWidth);
 digitalWrite(IN4, LOW);
}
