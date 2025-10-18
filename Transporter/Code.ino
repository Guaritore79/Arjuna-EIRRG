#include <PS4Controller.h>

int frontRight1 = 26;
int frontRight2 = 27;
int EnafrontRight = 14;

int frontLeft1 = 21;
int frontLeft2 = 22;
int EnafrontLeft = 23;

int backRight1 = 25;
int backRight2 = 33;
int EnabackRight = 32;

int backLeft1 = 19;
int backLeft2 = 18;
int EnabackLeft = 5;

int speed = 100;


void setup() {
  pinMode(frontRight1, OUTPUT);  
  pinMode(frontRight2, OUTPUT);
  pinMode(frontLeft1, OUTPUT);
  pinMode(frontLeft2, OUTPUT);
  pinMode(backRight1, OUTPUT);
  pinMode(backRight2, OUTPUT);
  pinMode(backLeft1, OUTPUT);
  pinMode(backLeft2, OUTPUT);

  pinMode(EnafrontRight, OUTPUT);
  pinMode(EnafrontLeft, OUTPUT);
  pinMode(EnabackRight, OUTPUT);
  pinMode(EnabackLeft, OUTPUT);

  PS4.begin("10:6f:d9:73:f2:92");
  Serial.println("Ready.");

  Serial.begin(115200);
}

void loop() {
  if (PS4.isConnected()) {
    Serial.println("Connected!");
  }

  if (PS4.Right() == 1){
    movingForward();
    Serial.println("maju");
  }

  delay(25);

}

void movingForward(){
  digitalWrite(frontRight1, HIGH);
  digitalWrite(frontRight2, LOW);
  analogWrite(EnafrontRight, speed);

  // Motor Depan Kiri maju
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontLeft2, LOW);
  analogWrite(EnafrontLeft, speed);
  
  // Motor Belakang Kanan maju
  digitalWrite(backRight1, HIGH);
  digitalWrite(backRight2, LOW);
  analogWrite(EnabackRight, speed);
  
  // Motor Belakang Kiri maju
  digitalWrite(backLeft1, HIGH);
  digitalWrite(backLeft2, LOW);
  analogWrite(EnabackLeft, speed);
}

 