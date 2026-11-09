#include "ThingBot.h"

ThingBotClass ThingBot;

static const uint8_t MOTOR_A[4] = {TB_M1_A, TB_M2_A, TB_M3_A, TB_M4_A};
static const uint8_t MOTOR_B[4] = {TB_M1_B, TB_M2_B, TB_M3_B, TB_M4_B};
static const uint8_t SERVO_CH[5] = {TB_SERVO_1, TB_SERVO_2, TB_SERVO_3, TB_SERVO_4, TB_SERVO_5};

#define SERVO_PULSE_MIN 102  // 0.5 ms @ 50 Hz
#define SERVO_PULSE_MAX 512  // 2.5 ms @ 50 Hz

bool ThingBotClass::begin() {
  Wire.begin();
  Wire.beginTransmission(0x40);
  bool found = (Wire.endTransmission() == 0);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  pinMode(TB_SW, INPUT);

  led(1, 0);
  led(2, 0);
  pwm.setPWM(TB_BUZZER, 0, 0);
  stop();
  return found;
}

// ---------------- LED ----------------
void ThingBotClass::led(uint8_t which, int percent) {
  uint8_t ch = (which == 2) ? TB_LED_2 : TB_LED_1;
  pwm.setPin(ch, map(constrain(percent, 0, 100), 0, 100, 0, 4095));
}

// ---------------- Nut SW ----------------
bool ThingBotClass::buttonHeld() {
  return digitalRead(TB_SW) == LOW;
}

bool ThingBotClass::buttonClicked() {
  bool raw = buttonHeld();
  if (raw != _lastRaw) {
    _lastRaw = raw;
    _lastChange = millis();
  }
  if (millis() - _lastChange > 25 && raw != _stable) {
    _stable = raw;
    return _stable;  // chi bao true o canh bam xuong
  }
  return false;
}

// ---------------- Coi ----------------
void ThingBotClass::beep(int ms) {
  pwm.setPWM(TB_BUZZER, 0, 2048);
  delay(ms);
  pwm.setPWM(TB_BUZZER, 0, 0);
}

void ThingBotClass::playNote(int hz, float beats) {
  int ms = (int)(beats * (60000.0 / _bpm));
  if (hz > 0) {
    pwm.setPWMFreq(constrain(hz, 24, 1526));
    pwm.setPWM(TB_BUZZER, 0, 2048);
  }
  delay(ms > 50 ? ms - 50 : ms);
  pwm.setPWM(TB_BUZZER, 0, 0);
  pwm.setPWMFreq(50);
  delay(50);
}

// ---------------- Dong co ----------------
void ThingBotClass::motor(uint8_t m, int speed) {
  if (m < 1 || m > 4) return;
  uint8_t a = MOTOR_A[m - 1], b = MOTOR_B[m - 1];
  int pulse = map(constrain(abs(speed), 0, 100), 0, 100, 0, 4095);
  pwm.setPWM(a, 0, speed < 0 ? pulse : 0);
  pwm.setPWM(b, 0, speed > 0 ? pulse : 0);
}

void ThingBotClass::drive(int left, int right) {
  motor(1, left);
  motor(2, right);
}

void ThingBotClass::stop() {
  for (uint8_t m = 1; m <= 4; m++) motor(m, 0);
}

// ---------------- Servo ----------------
int ThingBotClass::servoChannel(uint8_t s) {
  return (s >= 1 && s <= 5) ? SERVO_CH[s - 1] : -1;
}

void ThingBotClass::servoWrite(uint8_t idx, int deg) {
  deg = constrain(deg, 0, 180);
  pwm.setPWM(SERVO_CH[idx], 0, map(deg, 0, 180, SERVO_PULSE_MIN, SERVO_PULSE_MAX));
  _angle[idx] = deg;
}

void ThingBotClass::servo(uint8_t s, int deg) {
  if (servoChannel(s) < 0) return;
  _motion[s - 1].active = false;
  servoWrite(s - 1, deg);
}

void ThingBotClass::servoMove(uint8_t s, int deg, float seconds) {
  servoStart(s, deg, seconds);
  servoWait();
}

void ThingBotClass::servoStart(uint8_t s, int deg, float seconds) {
  if (servoChannel(s) < 0) return;
  Motion &m = _motion[s - 1];
  m.from = _angle[s - 1];
  m.to = constrain(deg, 0, 180);
  m.start = millis();
  m.dur = seconds > 0 ? (unsigned long)(seconds * 1000) : 0;
  m.active = true;
}

bool ThingBotClass::servoUpdate() {
  bool running = false;
  for (uint8_t i = 0; i < 5; i++) {
    Motion &m = _motion[i];
    if (!m.active) continue;
    unsigned long elapsed = millis() - m.start;
    if (m.dur == 0 || elapsed >= m.dur) {
      servoWrite(i, m.to);
      m.active = false;
    } else {
      servoWrite(i, m.from + (long)(m.to - m.from) * (long)elapsed / (long)m.dur);
      running = true;
    }
  }
  return running;
}

void ThingBotClass::servoWait() {
  while (servoUpdate()) delay(20);
}

void ThingBotClass::servoRelease(uint8_t s) {
  int ch = servoChannel(s);
  if (ch < 0) return;
  _motion[s - 1].active = false;
  pwm.setPWM(ch, 0, 4096);  // full-off
}

int ThingBotClass::servoAngle(uint8_t s) {
  return servoChannel(s) < 0 ? -1 : _angle[s - 1];
}

// ---------------- Sieu am ----------------
void ThingBotClass::ultrasonicPins(uint8_t trig, uint8_t echo) {
  _trig = trig;
  _echo = echo;
  _usReady = false;
}

float ThingBotClass::distanceCm() {
  if (!_usReady) {
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
    _usReady = true;
  }
  digitalWrite(_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);
  unsigned long t = pulseIn(_echo, HIGH, 30000);
  if (t == 0) return -1;
  return t * 0.0343f / 2.0f;
}
