/*
 * Chuong 2 - Den LED nhap nhay
 * LED_1, LED_2 noi vao chip PCA9685 (kenh 15, 13), khong noi thang vao ESP32.
 * Thu vien can cai: Adafruit PWM Servo Driver Library
 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define LED_1 15
#define LED_2 13

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();  // dia chi 0x40

void setup() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
}

void loop() {
  pwm.setPin(LED_1, 4095);  // 4095 = bat het muc
  pwm.setPin(LED_2, 0);     // 0    = tat
  delay(500);

  pwm.setPin(LED_1, 0);
  pwm.setPin(LED_2, 4095);
  delay(500);
}
