/*
 * Chuong 15 - Cam bien gia toc / con quay MPU6050 (I2C 0x68)
 * - Tinh goc nghieng truoc/sau (pitch) va trai/phai (roll)
 * - Nghieng qua 30 do -> LED bao huong; lat up (>70 do) -> coi bao dong
 * - Phat hien "va cham": gia toc tong thay doi dot ngot
 * Thu vien: Adafruit MPU6050, Adafruit Unified Sensor
 */
#include <ThingBot.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  ThingBot.begin();
  if (!mpu.begin()) {
    Serial.println("Khong tim thay MPU6050!");
    while (true) delay(1000);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = a.acceleration.x, ay = a.acceleration.y, az = a.acceleration.z;
  // Goc nghieng tinh tu vector trong luc (don vi do)
  float pitch = atan2(ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
  float roll  = atan2(ay, az) * 180.0 / PI;
  float total = sqrt(ax * ax + ay * ay + az * az);  // ~9.8 khi dung yen

  Serial.printf("pitch:%.0f roll:%.0f g:%.1f\n", pitch, roll, total);

  ThingBot.led(1, roll > 30 ? 100 : 0);   // nghieng mot ben
  ThingBot.led(2, roll < -30 ? 100 : 0);  // nghieng ben kia

  if (fabs(pitch) > 70 || fabs(roll) > 70) {
    ThingBot.beep(50);  // robot bi lat!
  }
  if (total > 20) {  // ~2g: va cham manh
    Serial.println("!!! VA CHAM !!!");
    ThingBot.playNote(NOTE_A5, 0.5);
  }
  delay(50);
}
