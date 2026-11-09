/*
 * Chuong 11 - Canh tay robot 3 khop
 * S1 = de xoay, S2 = khuyu tay, S3 = kep.
 * Bam SW: canh tay gap vat ben TRAI va dat sang ben PHAI.
 * Hoc: servoStart() + servoWait() de nhieu servo quay SONG SONG,
 *      mang 2 chieu luu "tu the" (pose).
 */
#include <ThingBot.h>

#define BASE  1
#define ELBOW 2
#define GRIP  3

#define GRIP_OPEN  40
#define GRIP_CLOSE 110

// Moi tu the: {goc de, goc khuyu}
const int HOME[2]  = {90, 60};
const int LEFT[2]  = {160, 130};
const int RIGHT[2] = {20, 130};
const int LIFT     = 60;

// Dua de va khuyu toi tu the trong `sec` giay, CUNG LUC
void goTo(const int pose[2], float sec) {
  ThingBot.servoStart(BASE, pose[0], sec);
  ThingBot.servoStart(ELBOW, pose[1], sec);
  ThingBot.servoWait();
}

void grip(bool close) {
  ThingBot.servoMove(GRIP, close ? GRIP_CLOSE : GRIP_OPEN, 0.4);
}

void lift() {
  ThingBot.servoMove(ELBOW, LIFT, 0.6);
}

void pickAndPlace() {
  grip(false);
  goTo(LEFT, 1.0);
  grip(true);
  lift();
  ThingBot.servoMove(BASE, RIGHT[0], 1.2);
  goTo(RIGHT, 0.6);
  grip(false);
  goTo(HOME, 1.0);
}

void setup() {
  ThingBot.begin();
  ThingBot.servo(BASE, HOME[0]);
  ThingBot.servo(ELBOW, HOME[1]);
  ThingBot.servo(GRIP, GRIP_OPEN);
}

void loop() {
  if (ThingBot.buttonClicked()) {
    ThingBot.led(1, 100);
    pickAndPlace();
    ThingBot.led(1, 0);
  }
}
