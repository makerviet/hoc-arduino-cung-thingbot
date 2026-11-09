/*
 * Chuong 5 - Coi va am nhac
 * Bam SW de choi bai "Happy Birthday".
 * Hoc: mang (array), sizeof, ham co tham so, tan so am thanh.
 *
 * Coi o kenh 14 cua PCA9685. De ra not nhac ta doi tan so PWM cua PCA9685.
 * LUU Y: tan so nay dung chung cho 16 kenh -> khong choi nhac khi servo dang chay.
 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define BUZZER 14
#define SW     3
#define LED_1  15

#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define AS4 466
#define C5 523

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int bpm = 120;

void playNote(int hz, float beats) {
  int ms = beats * 60000.0 / bpm;
  if (hz > 0) {
    pwm.setPWMFreq(hz);
    pwm.setPWM(BUZZER, 0, 2048);  // 50% = song vuong
  }
  delay(ms - 50);
  pwm.setPWM(BUZZER, 0, 0);
  pwm.setPWMFreq(50);
  delay(50);
}

const int melody[] = {C4, C4, D4, C4, F4, E4,
                      C4, C4, D4, C4, G4, F4,
                      C4, C4, C5, A4, F4, E4, D4,
                      AS4, AS4, A4, F4, G4, F4};
const float beats[] = {0.75, 0.25, 1, 1, 1, 2,
                       0.75, 0.25, 1, 1, 1, 2,
                       0.75, 0.25, 1, 1, 1, 1, 2,
                       0.75, 0.25, 1, 1, 1, 2};
const int N = sizeof(melody) / sizeof(melody[0]);

void setup() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  pinMode(SW, INPUT);
  playNote(C5, 0.25);  // bip bao san sang
}

void loop() {
  if (digitalRead(SW) == LOW) {
    for (int i = 0; i < N; i++) {
      pwm.setPin(LED_1, (i % 2) ? 4095 : 0);  // den nhay theo not
      playNote(melody[i], beats[i]);
    }
    pwm.setPin(LED_1, 0);
  }
}
