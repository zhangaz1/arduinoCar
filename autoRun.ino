#include <Servo.h> // 引入舵机库

Servo myservo;

int serialCode = 9600; // 输出串口波段
int myservoPin = 10; // 舵机转向针

int front = 90; // 正前方角度
int leftFront = 165; // 35; // 左前角度
int rightFront = 15; // 145; // 右前角度

int inputPin = 9;  // 定義超音波信號接收腳位
int outputPin = 8; // 定義超音波信號發射腳位

int pinLF = 15;   // 左轮前转针位
int pinLB = 14;   // 左轮后转针位

int pinRB = 17;  // 右轮前转针位
int pinRF = 16;  // 右轮后转针位

int lPwm = 5; // 左轮pwm针
int rPwm = 3; // 右轮pwm针

int leftSpeedRate = 1.95; // 左轮速度比值(调节左右轮速度不一样的问题)
int frontSpeedRate = 1.2; // 前行速度是其他速度的倍数(后退,左转/右转)
int minDistance = 40; // 30-50
int defaultSpeed = 90; // 60 - 100
int speedStep = 10;
int currentSpeed = defaultSpeed;

int readDistanceDelay = 200;
int delayTemp = 500;


void setup() {
  Serial.begin(serialCode);
  myservo.attach(myservoPin);
  myservo.write(front);

  pinMode(pinLB, OUTPUT); // 腳位 8 (PWM)
  pinMode(pinLF, OUTPUT); // 腳位 9 (PWM)
  pinMode(pinRB, OUTPUT); // 腳位 10 (PWM)
  pinMode(pinRF, OUTPUT); // 腳位 11 (PWM)

  pinMode(lPwm,  OUTPUT);  // 腳位 3 (PWM)
  pinMode(rPwm,  OUTPUT);  // 腳位 5 (PWM)

  pinMode(inputPin, INPUT);    // 定義超音波輸入腳位
  pinMode(outputPin, OUTPUT);  // 定義超音波輸出腳位

}


void loop() {
  check();
  delay(delayTemp);
}

void check() {
  int distanceFront = testFrontDistance();
  if (distanceFront > minDistance * 2) {
    currentSpeed += speedStep;
    goFront(currentSpeed);
  } else {
    currentSpeed = defaultSpeed;

    int distanceLeft = testLeftFrontDistance();
    int distanceRight = testRightFrontDistance();

    if (distanceLeft > distanceRight) {
      if (distanceLeft > minDistance) {
        turnLeft(currentSpeed);
      } else {
        goBack(currentSpeed);
      }
    } else {
      if (distanceRight > minDistance) {
        turnRight(currentSpeed);
      } else {
        goBack(currentSpeed);
      }
    }
  }
}

void turnLeft(int speed) {
  leftGoBack(speed);
  rightGoFront(speed);
}

void turnRight(int speed) {
  leftGoFront(speed);
  rightGoBack(speed);
}

void goFront(int speed) {
  speed *= frontSpeedRate;
  leftGoFront(speed);
  rightGoFront(speed);
}

void goBack(int speed) {
  leftGoBack(speed);
  rightGoBack(speed);
}

void leftGoFront(int speed) {
  digitalWrite(pinLF, HIGH);
  digitalWrite(pinLB, LOW);
  analogWrite(lPwm, speed * leftSpeedRate);
}
void leftGoBack(int speed) {
  digitalWrite(pinLF, LOW);
  digitalWrite(pinLB, HIGH);
  analogWrite(lPwm, speed * leftSpeedRate);
}

void rightGoFront(int speed) {
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinRB, LOW);
  analogWrite(rPwm, speed);
}
void rightGoBack(int speed) {
  digitalWrite(pinRF, LOW);
  digitalWrite(pinRB, HIGH);
  analogWrite(rPwm, speed);
}

int testFrontDistance() {
  return testDistance(front);
}
int testLeftFrontDistance() {
  return testDistance(leftFront);
}
int testRightFrontDistance() {
  return testDistance(rightFront);
}
int testDistance(int direction) {
  myservo.write(direction);
  delay(readDistanceDelay);

  digitalWrite(outputPin, LOW);   // 讓超聲波發射低電壓2μs
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);  // 讓超聲波發射高電壓10μs，這裡至少是10μs
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);    // 維持超聲波發射低電壓
  float distance = pulseIn(inputPin, HIGH);  // 讀差相差時間
  distance = distance / 5.8 / 10;  // 將時間轉為距離距离（單位：公分）

  Serial.print(direction);
  Serial.print('\n');
  Serial.print(distance);
  Serial.print('\n');
  Serial.print('\n');
  Serial.print('\n');
  return distance;
}
