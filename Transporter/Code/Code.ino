#include <PS4Controller.h>
#include <ESP32Servo.h>

Servo gripServo;
int servoPos = 0;

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

int lifter1 = 16;
int lifter2 = 17;
int enaLifter = 4; //motor lifter

int maxPWM = 240;

const int freq = 400;
const int resolution = 8;

const int pwmChannel0 = 4;
const int pwmChannel1 = 5;
const int pwmChannel2 = 6;
const int pwmChannel3 = 7;
const int pwmChannel4 = 8;
    
uint16_t vx, vy, vw;


void notify()
{
  if (!PS4.isConnected()) {
    stopAllMotors();
    return;
  }

  int rawX = PS4.LStickX();
  int rawY = PS4.LStickY();
  int rawW = PS4.RStickX();

  const int DEADZONE = 10;   // toleransi stick diam
  const int LIMIT = 90;      // batas maksimum

  // --- Terapkan deadzone ---
  if (abs(rawX) < DEADZONE) rawX = 0;
  if (abs(rawY) < DEADZONE) rawY = 0;
  if (abs(rawW) < DEADZONE) rawW = 0;

  // --- Batasi range agar tidak lebih dari ±80 ---
  rawX = constrain(rawX, -LIMIT, LIMIT);
  rawY = constrain(rawY, -LIMIT, LIMIT);
  rawW = constrain(rawW, -LIMIT, LIMIT);

  // --- Mapping ke kecepatan/pwm ---
  int vx = map(rawX, -LIMIT, LIMIT, -255, 255);
  int vy = -map(rawY, -LIMIT, LIMIT, -255, 255);
  int vw = -map(rawW, -LIMIT, LIMIT, -130, 130);

  int16_t v1 = mecanum_kinematic(1, vx, vy, vw);
  int16_t v2= mecanum_kinematic(2, vx, vy, vw);
  int16_t v3 = mecanum_kinematic(3, vx, vy, vw);
  int16_t v4 = mecanum_kinematic(4, vx, vy, vw);

  if(PS4.Triangle()) maxPWM = 240;
  if(PS4.Circle()) maxPWM = 225;


  v1 = constrain(v1, (-maxPWM - 15), (maxPWM + 15));
  v2 = constrain(v2, (-maxPWM - 15), (maxPWM + 15));
  v3 = constrain(v3, -maxPWM, maxPWM);
  v4 = constrain(v4, -maxPWM, maxPWM);

  setMotor(1, v1);
  setMotor(2, v2);
  setMotor(3, v3);
  setMotor(4, v4);

  if(PS4.Left()){
    digitalWrite(frontLeft1, LOW);
    digitalWrite(frontLeft2, HIGH);
    ledcWrite(pwmChannel0, 225);

    digitalWrite(frontRight1, LOW); 
    digitalWrite(frontRight2, HIGH);
    ledcWrite(pwmChannel1, 225);

    digitalWrite(backLeft1, LOW);
    digitalWrite(backLeft2, HIGH);
    ledcWrite(pwmChannel2, 225);

    digitalWrite(backRight1, LOW);
    digitalWrite(backRight2, HIGH);
    ledcWrite(pwmChannel3, 225); 
  }

  if(PS4.Right()){
    digitalWrite(frontLeft1, HIGH);
    digitalWrite(frontLeft2, LOW);
    ledcWrite(pwmChannel0, 225);

    digitalWrite(frontRight1, HIGH); 
    digitalWrite(frontRight2, LOW);
    ledcWrite(pwmChannel1, 225);

    digitalWrite(backLeft1, HIGH);
    digitalWrite(backLeft2, LOW);
    ledcWrite(pwmChannel2, 225);

    digitalWrite(backRight1, HIGH);
    digitalWrite(backRight2, LOW);
    ledcWrite(pwmChannel3, 225); 
  }

  Serial.print("v1 : "); Serial.println(v1);
  Serial.print("v2 : "); Serial.println(v2);
  Serial.print("v3 : "); Serial.println(v3);
  Serial.print("v4 : "); Serial.println(v4);

  static unsigned long lastServoTime = 0;
  const unsigned long servoInterval = 100;
  static bool servoInitialized = false;

  // Inisialisasi servo position hanya sekali
  if (!servoInitialized) {
    servoPos = 0;
    gripServo.write(servoPos);
    servoInitialized = true;
  }
  
  // if(millis() - lastServoTime >= servoInterval){

  //   bool up= PS4.L1();
  //   bool down= PS4.L2();

  //   if( up && !down && servoPos < 180){
  //     servoPos +=5;
  //   }

  //   if(down && !up && servoPos > 0){
  //     servoPos -=5;
  //   }
  //   gripServo.write(servoPos);
  //   lastServoTime = millis();

  // }


  if(PS4.L1()){
    gripServo.write(50);
  } 
  if(PS4.L2()){
    gripServo.write(0);
  }
  


  
  Serial.print("Grip : "); Serial.println(servoPos);

  if (PS4.R1()) {
    // L1 ditekan → lifter naik
    digitalWrite(lifter1, HIGH);
    digitalWrite(lifter2, LOW);
    ledcWrite(pwmChannel4, 100);
  } 
  else if (PS4.R2()) {
    // L2 ditekan → lifter turun
    digitalWrite(lifter1, LOW);
    digitalWrite(lifter2, HIGH);
    ledcWrite(pwmChannel4, 90);
  } 
  else {
    // Tidak ditekan → lifter berhenti
    digitalWrite(lifter1, LOW);
    digitalWrite(lifter2, LOW);
    ledcWrite(pwmChannel4, 0);
  }

}


void setup() 
{
  Serial.begin(115200);
  PS4.attach(notify);
  PS4.begin("6c:c8:40:8b:85:c2");

  gripServo.attach(12, 500, 2400); 
  gripServo.write(0);

  pinMode(frontRight1, OUTPUT);  
  pinMode(frontRight2, OUTPUT);
  pinMode(frontLeft1, OUTPUT);
  pinMode(frontLeft2, OUTPUT);
  pinMode(backRight1, OUTPUT);
  pinMode(backRight2, OUTPUT);
  pinMode(backLeft1, OUTPUT);
  pinMode(backLeft2, OUTPUT);

  pinMode(lifter1, OUTPUT);
  pinMode(lifter2, OUTPUT);

  ledcSetup(pwmChannel0, freq, resolution);
  ledcSetup(pwmChannel1, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  ledcSetup(pwmChannel3, freq, resolution);
  ledcSetup(pwmChannel4, freq, resolution);


  ledcAttachPin(EnafrontLeft, pwmChannel0);
  ledcAttachPin(EnafrontRight, pwmChannel1);
  ledcAttachPin(EnabackLeft, pwmChannel2);
  ledcAttachPin(EnabackRight, pwmChannel3);
  ledcAttachPin(enaLifter, pwmChannel4);

  Serial.println("Ready.");
}

void loop() 
{

}


void stopAllMotors() {
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, LOW);
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, LOW);
  digitalWrite(backRight1, LOW);
  digitalWrite(backRight2, LOW);
  digitalWrite(backLeft1, LOW);
  digitalWrite(backLeft2, LOW);
  digitalWrite(lifter1, LOW);
  digitalWrite(lifter2, LOW);
  ledcWrite(pwmChannel0, 0);
  ledcWrite(pwmChannel1, 0);
  ledcWrite(pwmChannel2, 0);
  ledcWrite(pwmChannel3, 0);
  ledcWrite(pwmChannel4, 0);

}


void setMotor(uint8_t motor, int16_t speed){
  uint8_t dirA = (speed >= 0);
  uint8_t dirB = (speed < 0);

  speed = abs(speed);

  switch(motor){
    case 1:
        digitalWrite(frontLeft1, dirB);
        digitalWrite(frontLeft2, dirA);
        ledcWrite(pwmChannel0, speed);
        break;
    case 2:
        digitalWrite(frontRight1, dirB); 
        digitalWrite(frontRight2, dirA);
        ledcWrite(pwmChannel1, speed);
        break;
    case 3:
        digitalWrite(backLeft1, dirB);
        digitalWrite(backLeft2, dirA);
        ledcWrite(pwmChannel2, speed);
        break;
    case 4:
        digitalWrite(backRight1, dirB);
        digitalWrite(backRight2, dirA);
        ledcWrite(pwmChannel3, speed);    
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

