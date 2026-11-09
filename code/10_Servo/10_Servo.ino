/*
 * Chuong 10 - Servo
 * S1 quet qua lai, dieu khien goc bang lenh go tu Serial Monitor:
 *   go "45"  + Enter -> S1 quay toi 45 do
 *   go "r"   + Enter -> tha long S1
 */
#include <ThingBot.h>

void setup() {
  Serial.begin(115200);
  ThingBot.begin();
  ThingBot.servo(1, 90);
  delay(500);

  // Quay ngay vs quay muot
  ThingBot.servo(1, 0);           delay(700);
  ThingBot.servo(1, 180);         delay(700);
  ThingBot.servoMove(1, 0, 1.5);  // 1.5 giay
  ThingBot.servoMove(1, 90, 0.8);

  Serial.println("Go goc 0..180 (hoac 'r' de tha long) roi Enter");
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line == "r") {
      ThingBot.servoRelease(1);
      Serial.println("Da tha long S1");
    } else if (line.length() > 0) {
      int deg = constrain(line.toInt(), 0, 180);
      ThingBot.servoMove(1, deg, 0.5);
      Serial.printf("S1 -> %d do\n", deg);
    }
  }
}
