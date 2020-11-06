#define IN1 5            //pinos Arduino ligados a entrada da Ponte H
#define IN2 4

void ligaMotor();

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  ligaMotor();
}

void ligaMotor() {
  analogWrite(IN1, HIGH);
  //digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}
