/*
 * Chuong 3 - Den "tho": sang dan, tat dan
 * Hoc: vong lap for, ham tu viet, ham map() va constrain().
 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define LED_1 15
#define LED_2 13

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Ham tu viet: dat do sang theo phan tram 0..100
void setLed(int led, int percent) {
  percent = constrain(percent, 0, 100);           // ep vao khoang 0..100
  int value = map(percent, 0, 100, 0, 4095);      // doi 0..100 -> 0..4095
  pwm.setPin(led, value);
}

void setup() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
}

void loop() {
  // LED_1 sang dan, LED_2 toi dan
  for (int i = 0; i <= 100; i++) {
    setLed(LED_1, i);
    setLed(LED_2, 100 - i);
    delay(15);
  }
  // nguoc lai
  for (int i = 100; i >= 0; i--) {
    setLed(LED_1, i);
    setLed(LED_2, 100 - i);
    delay(15);
  }
}
