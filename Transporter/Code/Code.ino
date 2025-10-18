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
  Serial.begin(115200);

  pinMode(frontRight1, OUTPUT);  
  pinMode(frontRight2, OUTPUT);
  pinMode(frontLeft1, OUTPUT);
  pinMode(frontLeft2, OUTPUT);
  pinMode(backRight1, OUTPUT);
  pinMode(backRight2, OUTPUT);
  pinMode(backLeft1, OUTPUT);
  pinMode(backLeft2, OUTPUT);

  PS4.begin("6c:c8:40:8b:85:c2");
  // Serial.println("Ready.");

  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, LOW);
  digitalWrite(backRight1, LOW);
  digitalWrite(backRight2, LOW);
  digitalWrite(backLeft1, LOW);
  digitalWrite(backLeft2, LOW);


}

void loop() {
  if (PS4.isConnected()) {
    // Serial.println("Connected!");
  }

  if (PS4.Triangle() == 1){
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

    Serial.println("Maju");

  }else{
    diam();
  }

  delay(25);

}

// void movingForward(){
  // digitalWrite(frontRight1, HIGH);
  // digitalWrite(frontRight2, LOW);
  // analogWrite(EnafrontRight, speed);

  // // Motor Depan Kiri maju
  // digitalWrite(frontLeft1, HIGH);
  // digitalWrite(frontLeft2, LOW);
  // analogWrite(EnafrontLeft, speed);
  
  // // Motor Belakang Kanan maju
  // digitalWrite(backRight1, HIGH);
  // digitalWrite(backRight2, LOW);
  // analogWrite(EnabackRight, speed);
  
  // // Motor Belakang Kiri maju
  // digitalWrite(backLeft1, HIGH);
  // digitalWrite(backLeft2, LOW);
  // analogWrite(EnabackLeft, speed);
// }

void diam(){
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, LOW);
  analogWrite(EnafrontRight, 0);

  // Motor Depan Kiri maju
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, LOW);
  analogWrite(EnafrontLeft, 0);
  
  // Motor Belakang Kanan maju
  digitalWrite(backRight1, LOW);
  digitalWrite(backRight2, LOW);
  analogWrite(EnabackRight, 0);
  
  // Motor Belakang Kiri maju
  digitalWrite(backLeft1, LOW);
  digitalWrite(backLeft2, LOW);
  analogWrite(EnabackLeft, 0);
}

 