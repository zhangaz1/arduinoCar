// #include <Time.h>
#include <Servo.h> // 引入舵机库

Servo myservo;

const int serialCode = 9600; // 输出串口波段

const int rPwm = 5; // 右轮pwm针
const int lPwm = 3; // 左轮pwm针

const int outputPin = 8; // 定義超音波信號發射腳位
const int inputPin = 9;  // 定義超音波信號接收腳位

const int myservoPin = 10; // 舵机转向针

const int pinLB = 16;   // 左轮后转针位
const int pinLF = 17;   // 左轮前转针位

const int pinRF = 14;  // 右轮后转针位
const int pinRB = 15;  // 右轮前转针位

const int front = 79; // 正前方角度
const int leftFront = 139; // 35; // 左前角度
const int rightFront = 19; // 145; // 右前角度

float leftSpeedRate = 0.88; // 左轮速度比值(调节左右轮速度不一样的问题)
float frontSpeedRate = 1.1; // 前行速度是其他速度的倍数(后退,左转/右转)
float backSpeedRate = 0.97; // 倒退速度比
float minDistance = 35; // 35; // 30-50
float frontDistanceRate = 1.5; // 前方安全距离与其他方向的安全距离比率
float defaultSpeed = 90; // 90; // 75; // 60 - 100
int speedStep = 3;
float currentSpeed = defaultSpeed;
float maxSpeed = 250 / frontSpeedRate;

int readDistanceDelay = 250;
int delayTemp = 100;

int limit = 2; // 前方距离小于最小距离连续次数
int added = 0; // 累计次数

String lastDirection = "front";

void setup() {
  if (leftSpeedRate > 1) {
    maxSpeed /= leftSpeedRate;
  }
  else {
    maxSpeed *= leftSpeedRate;
  }

  Serial.begin(serialCode);
  myservo.attach(myservoPin);
  myservo.write(front); // 超声波复位到前方

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
  checkServo();
  delay(delayTemp);
}


void checkServo() {
  int distanceFront = testFrontDistance();
  if (distanceFront > minDistance * frontDistanceRate) {
    added = 0;

    goFront(currentSpeed);
  }
  else {
    if (++added < limit) {
      return;
    }

    int distanceLeft = testLeftFrontDistance();
    int distanceRight = testRightFrontDistance();

    if (distanceLeft > distanceRight) {
      if (distanceLeft > minDistance) {
        turnLeft(currentSpeed);
      }
      else {
        goBack(currentSpeed);
      }
    }
    else {
      if (distanceRight > minDistance) {
        turnRight(currentSpeed);
      }
      else {
        goBack(currentSpeed);
      }
    }
  }
}

void turnLeft(float speed) {
  updateCurrentSpeed("left");
  leftGoBack(speed);
  rightGoFront(speed);
}

void turnRight(float speed) {
  updateCurrentSpeed("right");
  leftGoFront(speed);
  rightGoBack(speed);
}

void goFront(float speed) {
  updateCurrentSpeed("front");
  speed *= frontSpeedRate;
  leftGoFront(speed);
  rightGoFront(speed);
}

void goBack(float speed) {
  updateCurrentSpeed("back");
  leftGoBack(speed);
  rightGoBack(speed);
}

void updateCurrentSpeed(String direction) {
  if (lastDirection == direction) {
    currentSpeed += speedStep;
    if (currentSpeed > maxSpeed) {
      currentSpeed = maxSpeed;
    }
  } else {
    lastDirection = direction;
    currentSpeed = defaultSpeed;
  }
}

void leftGoFront(float speed) {
  digitalWrite(pinLF, HIGH);
  digitalWrite(pinLB, LOW);
  analogWrite(lPwm, speed * leftSpeedRate);
}
void leftGoBack(int speed) {
  digitalWrite(pinLF, LOW);
  digitalWrite(pinLB, HIGH);
  analogWrite(lPwm, speed * leftSpeedRate * backSpeedRate);
}

void rightGoFront(float speed) {
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinRB, LOW);
  analogWrite(rPwm, speed);
}
void rightGoBack(float speed) {
  digitalWrite(pinRF, LOW);
  digitalWrite(pinRB, HIGH);
  analogWrite(rPwm, speed * backSpeedRate);
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
float testDistance(int direction) {
  myservo.write(direction);
  delay(readDistanceDelay);

  digitalWrite(outputPin, LOW);   // 讓超聲波發射低電壓2μs
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);  // 讓超聲波發射高電壓10μs，這裡至少是10μs
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);    // 維持超聲波發射低電壓
  float distance = pulseIn(inputPin, HIGH);  // 讀差相差時間
  distance = distance / 5.8 / 10;  // 將時間轉為距離距离（單位：公分）

  logDistanceTest(direction, distance);

  return distance;
}

void logDistanceTest(int direction, float distance) {
  //  Serial.print(now());
  Serial.print(direction);
  Serial.print(": ");
  Serial.print(distance);
  Serial.print('\n');
}












