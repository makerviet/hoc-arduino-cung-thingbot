/*
 * Chuong 12 - Cam bien sieu am HC-SR04 ("cam bien lui xe")
 * TRIG -> IO0, ECHO -> IO4
 * Vat cang gan, coi bip cang nhanh; < 10 cm thi keu lien tuc.
 * Mo Tools > Serial Plotter de xem do thi khoang cach.
 */
#include <ThingBot.h>

// Loc nhieu: lay trung vi (median) cua 3 lan do
float readFiltered() {
  float a = ThingBot.distanceCm(); delay(10);
  float b = ThingBot.distanceCm(); delay(10);
  float c = ThingBot.distanceCm();
  if (a < 0) a = 999;
  if (b < 0) b = 999;
  if (c < 0) c = 999;
  return max(min(a, b), min(max(a, b), c));
}

void setup() {
  Serial.begin(115200);
  ThingBot.begin();
}

void loop() {
  float d = readFiltered();
  Serial.printf("khoang_cach:%.1f\n", d);  // dang "ten:gia_tri" cho Serial Plotter

  if (d < 10) {
    ThingBot.pwm.setPin(TB_BUZZER, 2048);  // keu lien tuc
    ThingBot.led(1, 100);
    delay(50);
  } else if (d < 60) {
    ThingBot.pwm.setPin(TB_BUZZER, 0);
    ThingBot.led(1, 0);
    ThingBot.beep(40);
    delay(map((int)d, 10, 60, 50, 600));  // gan -> nghi ngan -> bip nhanh
  } else {
    ThingBot.pwm.setPin(TB_BUZZER, 0);
    ThingBot.led(1, 0);
    delay(100);
  }
}
