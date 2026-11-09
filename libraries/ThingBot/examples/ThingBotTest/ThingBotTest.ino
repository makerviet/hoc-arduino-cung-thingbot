// Kiem tra nhanh toan bo phan cung tren mach ThingBot.
// Bam SW de chuyen sang buoc tiep theo. Mo Serial Monitor 115200 de xem huong dan.
#include <ThingBot.h>

void waitClick(const char *msg) {
  Serial.println(msg);
  while (!ThingBot.buttonClicked()) delay(5);
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  if (!ThingBot.begin()) Serial.println("CANH BAO: khong thay PCA9685 (0x40)!");

  waitClick("[1] Bam SW de thu LED");
  ThingBot.led(1, 100); delay(500); ThingBot.led(1, 0);
  ThingBot.led(2, 100); delay(500); ThingBot.led(2, 0);

  waitClick("[2] Bam SW de thu coi");
  ThingBot.playNote(NOTE_C5, 0.5);
  ThingBot.playNote(NOTE_E5, 0.5);
  ThingBot.playNote(NOTE_G5, 0.5);

  waitClick("[3] Bam SW de thu dong co M1..M4");
  for (int m = 1; m <= 4; m++) {
    ThingBot.motor(m, 60);  delay(700);
    ThingBot.motor(m, -60); delay(700);
    ThingBot.motor(m, 0);
  }

  waitClick("[4] Bam SW de thu servo S1..S5");
  for (int s = 1; s <= 5; s++) {
    ThingBot.servoMove(s, 30, 0.4);
    ThingBot.servoMove(s, 150, 0.6);
    ThingBot.servoMove(s, 90, 0.4);
  }
  Serial.println("Xong! In khoang cach sieu am (TRIG IO0, ECHO IO4):");
}

void loop() {
  Serial.println(ThingBot.distanceCm());
  delay(300);
}
