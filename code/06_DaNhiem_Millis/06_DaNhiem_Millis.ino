/*
 * Chuong 6 - Lam nhieu viec cung luc voi millis()
 * - LED_1 nhay moi 300 ms, LED_2 nhay moi 1000 ms (doc lap nhau)
 * - Nut SW luon phan hoi ngay lap tuc (bam -> bip ngan)
 * Hoc: vi sao delay() lam chuong trinh "dung hinh", ky thuat dinh thoi bang millis().
 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SW     3
#define LED_1  15
#define LED_2  13
#define BUZZER 14

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

unsigned long lastLed1 = 0, lastLed2 = 0, beepUntil = 0;
bool led1 = false, led2 = false, lastPressed = false;

void setup() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  pinMode(SW, INPUT);
}

void loop() {
  unsigned long now = millis();  // so ms tu luc khoi dong

  // Viec 1: LED_1 moi 300 ms
  if (now - lastLed1 >= 300) {
    lastLed1 = now;
    led1 = !led1;
    pwm.setPin(LED_1, led1 ? 4095 : 0);
  }

  // Viec 2: LED_2 moi 1000 ms
  if (now - lastLed2 >= 1000) {
    lastLed2 = now;
    led2 = !led2;
    pwm.setPin(LED_2, led2 ? 4095 : 0);
  }

  // Viec 3: nut bam -> bip 80 ms ma KHONG dung delay
  bool pressed = digitalRead(SW) == LOW;
  if (pressed && !lastPressed) {
    pwm.setPin(BUZZER, 2048);
    beepUntil = now + 80;
  }
  lastPressed = pressed;
  if (beepUntil && now >= beepUntil) {
    pwm.setPin(BUZZER, 0);
    beepUntil = 0;
  }
}
