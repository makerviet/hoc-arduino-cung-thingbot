/*
 * Chuong 4 - Nut bam SW
 * - Giu nut: LED_2 sang
 * - Moi lan bam: dao trang thai LED_1 va dem so lan bam
 * Hoc: digitalRead, if/else, bien bool, phat hien canh bam, chong doi phim.
 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SW    3    // GPIO3 cua ESP32-C3, bam = LOW
#define LED_1 15
#define LED_2 13

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

bool led1On = false;
bool lastPressed = false;
int clicks = 0;

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  pinMode(SW, INPUT);
}

void loop() {
  bool pressed = (digitalRead(SW) == LOW);

  if (pressed) {
    pwm.setPin(LED_2, 4095);
  } else {
    pwm.setPin(LED_2, 0);
  }

  // Canh bam: lan truoc chua bam, lan nay dang bam
  if (pressed && !lastPressed) {
    led1On = !led1On;
    pwm.setPin(LED_1, led1On ? 4095 : 0);
    clicks++;
    Serial.print("So lan bam: ");
    Serial.println(clicks);
    delay(30);  // chong doi phim
  }
  lastPressed = pressed;
}
