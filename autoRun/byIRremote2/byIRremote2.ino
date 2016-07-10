/*
   IRRemote 红外遥控码测试
     範例 1.2: 顯示紅外線協定種類，如 NEC, Sony SIRC, Philips RC5, Philips RC6 等協定
*/
#include <IRremote.h>                    // 引用 IRRemote 函式庫

const int irReceiverPin = 7;             // 紅外線接收器 OUTPUT 訊號接在 pin 2

const int serialCode = 9600; // 输出串口波段

const int rPwm = 3; // 右轮pwm针
const int lPwm = 5; // 左轮pwm针

const int outputPin = 8; // 定義超音波信號發射腳位
const int inputPin = 9;  // 定義超音波信號接收腳位

const int myservoPin = 10; // 舵机转向针

const int pinLB = 14;   // 左轮后转针位
const int pinLF = 15;   // 左轮前转针位

const int pinRF = 16;  // 右轮后转针位
const int pinRB = 17;  // 右轮前转针位


IRrecv irrecv(irReceiverPin);            // 定義 IRrecv 物件來接收紅外線訊號
decode_results results;                  // 解碼結果將放在 decode_results 結構的 result 變數裏

void setup()
{
  Serial.begin(9600);                     // 開啟 Serial port, 通訊速率為 9600 bps
  irrecv.enableIRIn();                   // 啟動紅外線解碼

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
  checkRremote();
}

void checkRremote() {
  if (irrecv.decode(&results)) {         // 解碼成功，收到一組紅外線訊號
    //    showIRProtocol(&results);            // 顯示紅外線協定種類
    //    Serial.print(results.value, HEX);
    //    Serial.print("\n");
    long val = results.value;
    if (val == 0x00FF629D) {
      digitalWrite(pinLF, HIGH);
      digitalWrite(pinLB, LOW);
      analogWrite(lPwm, 100);
      
      digitalWrite(pinRF, HIGH);
      digitalWrite(pinRB, LOW);
      analogWrite(rPwm, 100);
    }
    else if (val == 0x00FF02FD) {
      digitalWrite(pinRF, LOW);
      digitalWrite(pinRB, HIGH);
      analogWrite(rPwm, 100);
      
      digitalWrite(pinLF, LOW);
      digitalWrite(pinLB, HIGH);
      analogWrite(lPwm, 100);
    }
    irrecv.resume();                     // 繼續收下一組紅外線訊號
  }
}

void showIRProtocol(decode_results *results) // 顯示紅外線協定種類
{
  Serial.print("Protocol: ");

  // 判斷紅外線協定種類
  switch (results->decode_type) {
    case NEC:
      Serial.print("NEC");
      break;
    case SONY:
      Serial.print("SONY");
      break;
    case RC5:
      Serial.print("RC5");
      break;
    case RC6:
      Serial.print("RC6");
      break;
    default:
      Serial.print("Unknown encoding");
      Serial.print(results->decode_type);
  }

  // 把紅外線編碼印到 Serial port
  Serial.print(", irCode: ");
  Serial.print(results->value, HEX);    // 紅外線編碼
  Serial.print(",  bits: ");
  Serial.println(results->bits);        // 紅外線編碼位元數
}

