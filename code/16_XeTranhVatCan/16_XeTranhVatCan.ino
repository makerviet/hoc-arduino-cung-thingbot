/*
 * Chuong 16 - Xe tranh vat can dung "may trang thai" (state machine)
 * M1 trai, M2 phai, sieu am TRIG IO0 / ECHO IO4.
 * Bam SW de bat / tat. Khong dung delay() trong qua trinh chay
 * -> nut bam luon phan hoi ngay, de them tinh nang moi.
 */
#include <ThingBot.h>

#define SPEED        60
#define TURN_SPEED   55
#define OBSTACLE_CM  25
#define BACK_MS      400

enum State { IDLE, FORWARD, BACKING, TURNING };
State state = IDLE;
unsigned long stateStart = 0;
unsigned long turnMs = 500;
int turnDir = 1;  // 1 = phai, -1 = trai

const char *stateName(State s) {
  switch (s) {
    case IDLE: return "IDLE";
    case FORWARD: return "FORWARD";
    case BACKING: return "BACKING";
    default: return "TURNING";
  }
}

void enter(State s) {
  state = s;
  stateStart = millis();
  Serial.printf("-> %s\n", stateName(s));
  switch (s) {
    case IDLE:
      ThingBot.stop();
      ThingBot.led(1, 0);
      ThingBot.led(2, 0);
      break;
    case FORWARD:
      ThingBot.drive(SPEED, SPEED);
      ThingBot.led(1, 100);
      ThingBot.led(2, 0);
      break;
    case BACKING:
      ThingBot.drive(-SPEED, -SPEED);
      ThingBot.led(2, 100);
      break;
    case TURNING:
      turnDir = random(2) ? 1 : -1;
      turnMs = random(300, 700);
      ThingBot.drive(TURN_SPEED * turnDir, -TURN_SPEED * turnDir);
      break;
  }
}

unsigned long lastMeasure = 0;
float distance = 999;

void setup() {
  Serial.begin(115200);
  ThingBot.begin();
  randomSeed(esp_random());
  enter(IDLE);
}

void loop() {
  unsigned long now = millis();

  // Bat / tat bat ky luc nao
  if (ThingBot.buttonClicked()) {
    ThingBot.beep(60);
    enter(state == IDLE ? FORWARD : IDLE);
  }

  // Do khoang cach moi 40 ms
  if (now - lastMeasure >= 40) {
    lastMeasure = now;
    float d = ThingBot.distanceCm();
    distance = (d < 0) ? 999 : d;
  }

  switch (state) {
    case IDLE:
      break;
    case FORWARD:
      if (distance < OBSTACLE_CM) enter(BACKING);
      break;
    case BACKING:
      if (now - stateStart >= BACK_MS) enter(TURNING);
      break;
    case TURNING:
      if (now - stateStart >= turnMs) enter(FORWARD);
      break;
  }
}
