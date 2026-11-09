/*
 * ThingBot.h - Thu vien ho tro mach ThingBot (ESP32-C3 + PCA9685)
 * Sach "Hoc Arduino cung ThingBot"
 *
 * Gom gon cac thao tac hay dung: LED, nut SW, coi, dong co, servo, sieu am.
 * Ban chat ben trong van la cac lenh pwm.setPWM() da hoc o Phan 1 & 2.
 */
#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ---------- Kenh PCA9685 ----------
#define TB_M1_A 2
#define TB_M1_B 3
#define TB_M2_A 4
#define TB_M2_B 5
#define TB_M3_A 7
#define TB_M3_B 6
#define TB_M4_A 1
#define TB_M4_B 0
#define TB_SERVO_1 12
#define TB_SERVO_2 11
#define TB_SERVO_3 10
#define TB_SERVO_4 9
#define TB_SERVO_5 8
#define TB_BUZZER 14
#define TB_LED_1 15
#define TB_LED_2 13

// ---------- Chan GPIO ESP32-C3 ----------
#define TB_SW 3
#define TB_TRIG 0
#define TB_ECHO 4
#define TB_DHT 4
#define TB_PS2_DAT 7
#define TB_PS2_CMD 2
#define TB_PS2_SEL 10
#define TB_PS2_CLK 6

// ---------- Not nhac (Hz) ----------
#define NOTE_REST 0
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988
#define NOTE_C6 1047

class ThingBotClass {
 public:
  // Khoi dong mach. Tra ve false neu khong thay chip PCA9685 tren I2C.
  bool begin();

  // ----- LED -----
  void led(uint8_t which, int percent);  // which: 1 hoac 2, percent 0..100

  // ----- Nut SW -----
  bool buttonHeld();     // true khi dang giu nut
  bool buttonClicked();  // true DUNG 1 LAN moi lan bam (da chong doi phim)

  // ----- Coi & am nhac -----
  void beep(int ms = 80);
  void setTempo(int bpm) { _bpm = bpm; }
  void playNote(int hz, float beats);  // hz = 0 la nghi

  // ----- Dong co DC -----
  void motor(uint8_t m, int speed);  // m: 1..4, speed: -100..100
  void drive(int left, int right);   // xe 2 banh: M1 trai, M2 phai
  void stop();                       // dung ca 4 dong co

  // ----- Servo -----
  void servo(uint8_t s, int deg);                      // s: 1..5, quay ngay
  void servoMove(uint8_t s, int deg, float seconds);   // quay muot (cho den khi xong)
  void servoStart(uint8_t s, int deg, float seconds);  // quay muot chay nen
  bool servoUpdate();                                  // goi lien tuc trong loop()
  void servoWait();                                    // cho moi servo quay xong
  void servoRelease(uint8_t s);                        // tha long
  int servoAngle(uint8_t s);

  // ----- Cam bien sieu am -----
  void ultrasonicPins(uint8_t trig, uint8_t echo);
  float distanceCm();  // -1 neu khong do duoc

  Adafruit_PWMServoDriver pwm;  // truy cap truc tiep neu can

 private:
  int servoChannel(uint8_t s);
  void servoWrite(uint8_t idx, int deg);

  int _bpm = 120;
  bool _lastRaw = false;
  bool _stable = false;
  unsigned long _lastChange = 0;
  uint8_t _trig = TB_TRIG, _echo = TB_ECHO;
  bool _usReady = false;

  struct Motion {
    int from, to;
    unsigned long start, dur;
    bool active;
  };
  int _angle[5] = {90, 90, 90, 90, 90};
  Motion _motion[5] = {};
};

extern ThingBotClass ThingBot;
