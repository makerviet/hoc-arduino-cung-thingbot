/*
 * Chuong 8 - Dong co DC
 * Bam SW de doi che do toc do cua M1: dung -> cham -> vua -> nhanh -> lui -> dung ...
 * Phan "ben trong" (khong dung thu vien) duoc giai thich trong sach.
 */
#include <ThingBot.h>

const int speeds[] = {0, 30, 60, 100, -60};
const char *names[] = {"Dung", "Cham", "Vua", "Nhanh", "Lui"};
const int NUM_MODES = 5;
int mode = 0;

void setup() {
  Serial.begin(115200);
  ThingBot.begin();
  Serial.println("Bam SW de doi toc do dong co M1");
}

void loop() {
  if (ThingBot.buttonClicked()) {
    mode = (mode + 1) % NUM_MODES;
    // Tang/giam toc tu tu de dong co va pin "de chiu" hon
    int from = speeds[(mode + NUM_MODES - 1) % NUM_MODES];
    int to = speeds[mode];
    int step = (to > from) ? 5 : -5;
    for (int s = from; s != to; s += step) {
      ThingBot.motor(1, s);
      delay(20);
    }
    ThingBot.motor(1, to);
    ThingBot.led(1, abs(to));
    Serial.printf("M1: %s (%d%%)\n", names[mode], to);
  }
}
