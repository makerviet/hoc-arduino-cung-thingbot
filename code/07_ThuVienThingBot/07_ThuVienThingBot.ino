/*
 * Chuong 7 - Thu vien ThingBot
 * Cung lam cac viec cua Chuong 2-6 nhung code ngan hon nhieu nho thu vien.
 * Cai dat: chep thu muc libraries/ThingBot vao Documents/Arduino/libraries
 */
#include <ThingBot.h>

int level = 0;  // muc sang 0..4

void setup() {
  Serial.begin(115200);
  if (!ThingBot.begin()) {
    Serial.println("Khong tim thay PCA9685!");
  }
  ThingBot.beep();
}

void loop() {
  if (ThingBot.buttonClicked()) {
    level = (level + 1) % 5;          // 0,1,2,3,4,0,...
    ThingBot.led(1, level * 25);      // 0%, 25%, 50%, 75%, 100%
    ThingBot.led(2, 100 - level * 25);
    ThingBot.playNote(NOTE_C5 + level * 100, 0.25);
    Serial.printf("Muc sang: %d%%\n", level * 25);
  }
}
