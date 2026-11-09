/*
 * Chuong 9 - Xe hai banh: di theo hinh vuong
 * M1 = banh trai, M2 = banh phai.
 * Bam SW: xe di 4 canh hinh vuong roi dung.
 * Can chinh TURN_90_MS cho den khi xe re dung 90 do tren mat san cua ban.
 */
#include <ThingBot.h>

#define SPEED       60
#define SIDE_MS     1200   // thoi gian di 1 canh
#define TURN_90_MS  420    // thoi gian xoay 90 do (CAN CHINH)

void forward(int ms)   { ThingBot.drive(SPEED, SPEED);   delay(ms); ThingBot.stop(); }
void backward(int ms)  { ThingBot.drive(-SPEED, -SPEED); delay(ms); ThingBot.stop(); }
void turnLeft(int ms)  { ThingBot.drive(-SPEED, SPEED);  delay(ms); ThingBot.stop(); }
void turnRight(int ms) { ThingBot.drive(SPEED, -SPEED);  delay(ms); ThingBot.stop(); }

void square() {
  for (int i = 0; i < 4; i++) {
    forward(SIDE_MS);
    delay(200);
    turnRight(TURN_90_MS);
    delay(200);
  }
}

void setup() {
  ThingBot.begin();
}

void loop() {
  if (ThingBot.buttonClicked()) {
    ThingBot.led(1, 100);
    delay(1000);  // kip bo tay ra
    square();
    ThingBot.led(1, 0);
    ThingBot.beep(200);
  }
}
