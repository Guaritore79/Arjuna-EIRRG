#include <PS4Controller.h>

int frontRight1 = 25;
int frontRight2 = 33;
int EnafrontRight = 32; // motor2

int frontLeft1 = 18;
int frontLeft2 = 19;
int EnafrontLeft = 5; //motor1

int backRight1 = 27;
int backRight2 = 26;
int EnabackRight = 14; //motor4

int backLeft1 = 21 ;
int backLeft2 = 22;
int EnabackLeft = 23; //motor3

uint16_t vx, vy, vw;

unsigned long previousDebugTime = 0;
const long debugInterval = 500;


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
  // if (PS4.isConnected()) {
  //   // Serial.println("Connected!");
  // }

  vx = map(control_drift(PS4.LStickX(), 12), -128, 127, -255, 255);
  vy = -map(control_drift(PS4.LStickY(), 12), -128, 127, -255, 255);
  vw = -map(control_drift(PS4.RStickX(), 12), -128, 127, -255, 255);

  int16_t v1 = mecanum_kinematic(1, vx, vy, vw);
  int16_t v2= mecanum_kinematic(2, vx, vy, vw);
  int16_t v3 = mecanum_kinematic(3, vx, vy, vw);
  int16_t v4 = mecanum_kinematic(4, vx, vy, vw);

  setMotor(1, v1);
  setMotor(2, v2);
  setMotor(3, v3);
  setMotor(4, v4);

}

void setMotor(uint8_t motor, int16_t speed){
  uint8_t dirA = (speed >= 0);
  uint8_t dirB = (speed < 0);

  speed = abs(speed);

  // unsigned long currentTime = millis();
  // if (currentTime - previousDebugTime >= debugInterval) {
  //   previousDebugTime = currentTime;
    
  //   Serial.print("Motor ");
  //   Serial.print(motor);
  //   Serial.print(": PWM = ");
  //   Serial.print(speed);
  //   Serial.print(", DirA = ");
  //   Serial.print(dirA);
  //   Serial.print(", DirB = ");
  //   Serial.println(dirB);
  // }

  switch(motor){
    case 1:
        digitalWrite(frontLeft1, dirB);
        digitalWrite(frontLeft2, dirA);
        analogWrite(EnafrontLeft, speed);
        break;
    case 2:
        digitalWrite(frontRight1, dirB); 
        digitalWrite(frontRight2, dirA);
        analogWrite(EnafrontRight, speed);
        break;
    case 3:
        digitalWrite(backLeft1, dirB);
        digitalWrite(backLeft2, dirA);
        analogWrite(EnabackLeft, speed);
        break;
    case 4:
        digitalWrite(backRight1, dirB);
        digitalWrite(backRight2, dirA);
        analogWrite(EnabackRight, speed);
        break;
    default:
        break;
  }
}

int16_t mecanum_kinematic(uint8_t motor, int16_t vx, int16_t vy, int16_t vw){
  switch(motor){
    case 1: return -vx + vy + vw;
    case 2: return -vx - vy + vw;    
    case 3: return -vx - vy - vw;
    case 4: return -vx + vy - vw;
    default : return 0;
  }
}

int16_t control_drift(int16_t value, int16_t limit){
  if (abs(value) < limit){
    return 0;
  }else if(value > 0){
    return value - limit;
  }else{
    return value + limit;
  }
}