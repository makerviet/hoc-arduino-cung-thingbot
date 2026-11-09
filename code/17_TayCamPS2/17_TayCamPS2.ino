/*
 * Chuong 17 - Xe dieu khien bang tay cam PS2 khong day (RF 2.4 GHz)
 *   Can TRAI      : tien/lui + re (lai kieu "arcade")
 *   Phim mui ten  : di voi toc do co dinh
 *   R1 / R2       : servo S1 kep mo / dong
 *   SELECT        : bat/tat che do cham (LED_2 sang)
 *   START         : coi
 * Bo thu PS2 cam vao cong PS2: DAT IO7, CMD IO2, SEL IO10, CLK IO6.
 * Thu vien: PS2X_lib
 */
#include <ThingBot.h>
#include <PS2X_lib.h>

#define DEADZONE  12
#define GRIP_OPEN  40
#define GRIP_CLOSE 110

PS2X ps2x;
bool ps2Ok = false;
bool slowMode = false;

int stick(int raw) {  // 0..255 -> -100..100, co vung chet o giua
  int v = raw - 128;
  if (abs(v) < DEADZONE) return 0;
  return constrain(map(v, -128, 127, -100, 100), -100, 100);
}

void setup() {
  Serial.begin(115200);
  ThingBot.begin();
  ThingBot.servo(1, GRIP_OPEN);

  for (int i = 1; i <= 10 && !ps2Ok; i++) {
    delay(500);
    int err = ps2x.config_gamepad(TB_PS2_CLK, TB_PS2_CMD, TB_PS2_SEL, TB_PS2_DAT, false, false);
    Serial.printf("Ket noi PS2 lan %d: ma loi %d\n", i, err);
    ps2Ok = (err == 0);
  }
  if (ps2Ok) {
    ThingBot.led(1, 100);
    ThingBot.playNote(NOTE_C5, 0.25);
    ThingBot.playNote(NOTE_G5, 0.5);
  } else {
    Serial.println("Khong ket noi duoc tay cam! Kiem tra bo thu va bat tay cam.");
  }
}

void loop() {
  if (!ps2Ok) {
    ThingBot.led(1, (millis() / 250) % 2 ? 100 : 0);  // nhay = loi
    return;
  }
  ps2x.read_gamepad(false, 0);

  int throttle = -stick(ps2x.Analog(PSS_LY));
  int turn = stick(ps2x.Analog(PSS_LX));

  if (ps2x.Button(PSB_PAD_UP))    throttle = 70;
  if (ps2x.Button(PSB_PAD_DOWN))  throttle = -70;
  if (ps2x.Button(PSB_PAD_LEFT))  turn = -60;
  if (ps2x.Button(PSB_PAD_RIGHT)) turn = 60;

  if (ps2x.ButtonPressed(PSB_SELECT)) {
    slowMode = !slowMode;
    ThingBot.led(2, slowMode ? 100 : 0);
  }

  int left = constrain(throttle + turn, -100, 100);
  int right = constrain(throttle - turn, -100, 100);
  if (slowMode) {
    left /= 3;
    right /= 3;
  }
  ThingBot.drive(left, right);

  if (ps2x.ButtonPressed(PSB_R1)) ThingBot.servoStart(1, GRIP_OPEN, 0.3);
  if (ps2x.ButtonPressed(PSB_R2)) ThingBot.servoStart(1, GRIP_CLOSE, 0.3);
  ThingBot.servoUpdate();

  ThingBot.pwm.setPin(TB_BUZZER, ps2x.Button(PSB_START) ? 2048 : 0);
  delay(20);
}
