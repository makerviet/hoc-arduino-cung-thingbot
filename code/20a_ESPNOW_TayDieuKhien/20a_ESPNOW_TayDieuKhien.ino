/*
 * Chuong 20a - TAY DIEU KHIEN RF tu lam (ESP-NOW)
 * Nap vao ThingBot thu NHAT co gan MPU6050 (cong I2C).
 * Nghieng tay cam: truoc/sau = tien/lui, trai/phai = re.
 * Giu SW = coi; bam SW = doi che do (dung o Chuong 21).
 *
 * ESP-NOW: giao thuc vo tuyen 2.4 GHz cua Espressif, khong can router Wi-Fi,
 * do tre vai ms, tam xa ~100 m ngoai troi.
 * Ca lop dung chung kenh: moi nhom dat ROBOT_ID khac nhau (1..255)!
 *
 * Thu vien: Adafruit MPU6050, Adafruit Unified Sensor
 */
#include <ThingBot.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define ROBOT_ID    1     // PHAI GIONG ben robot
#define RF_CHANNEL  1     // kenh Wi-Fi 1..13, PHAI GIONG ben robot
#define MAX_TILT    35.0  // nghieng 35 do = toc do toi da
#define DEAD_TILT   7.0   // nghieng duoi 7 do = dung yen
#define INVERT_THROTTLE false
#define INVERT_TURN     false

// Goi tin gui di - PHAI GIONG HET ben robot
struct __attribute__((packed)) RemotePacket {
  uint16_t magic;       // 0x7B07: nhan dien goi tin cua sach nay
  uint8_t robotId;
  int8_t throttle;      // -100..100
  int8_t turn;          // -100..100
  uint8_t buttons;      // bit0 = dang giu SW
  uint8_t clickCount;   // tang 1 moi lan bam SW
};

const uint8_t BROADCAST[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
Adafruit_MPU6050 mpu;
float pitch0 = 0, roll0 = 0;  // tu the "thang bang" luc khoi dong
uint8_t clicks = 0;

void readTilt(float &pitch, float &roll) {
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);
  float ax = a.acceleration.x, ay = a.acceleration.y, az = a.acceleration.z;
  pitch = atan2(ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
  roll = atan2(ay, az) * 180.0 / PI;
}

int tiltToPercent(float deg) {
  if (fabs(deg) < DEAD_TILT) return 0;
  float sign = deg > 0 ? 1 : -1;
  float v = (fabs(deg) - DEAD_TILT) / (MAX_TILT - DEAD_TILT) * 100.0;
  return (int)(sign * constrain(v, 0, 100));
}

void setup() {
  Serial.begin(115200);
  ThingBot.begin();
  if (!mpu.begin()) {
    Serial.println("Khong tim thay MPU6050!");
    while (true) { ThingBot.led(2, 100); delay(200); ThingBot.led(2, 0); delay(200); }
  }
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Hieu chuan: giu tay cam nam ngang trong 1 giay
  ThingBot.led(2, 100);
  float p, r;
  for (int i = 0; i < 20; i++) {
    readTilt(p, r);
    pitch0 += p / 20;
    roll0 += r / 20;
    delay(50);
  }
  ThingBot.led(2, 0);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(RF_CHANNEL, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Loi khoi dong ESP-NOW");
    return;
  }
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, BROADCAST, 6);
  peer.channel = RF_CHANNEL;
  peer.ifidx = WIFI_IF_STA;
  peer.encrypt = false;
  esp_now_add_peer(&peer);

  Serial.printf("Tay dieu khien san sang. ROBOT_ID=%d, kenh %d\n", ROBOT_ID, RF_CHANNEL);
  ThingBot.beep();
}

void loop() {
  if (ThingBot.buttonClicked()) clicks++;

  float p, r;
  readTilt(p, r);
  int throttle = tiltToPercent(p - pitch0);
  int turn = tiltToPercent(r - roll0);
  if (INVERT_THROTTLE) throttle = -throttle;
  if (INVERT_TURN) turn = -turn;

  RemotePacket pkt;
  pkt.magic = 0x7B07;
  pkt.robotId = ROBOT_ID;
  pkt.throttle = throttle;
  pkt.turn = turn;
  pkt.buttons = ThingBot.buttonHeld() ? 1 : 0;
  pkt.clickCount = clicks;

  esp_err_t res = esp_now_send(BROADCAST, (uint8_t *)&pkt, sizeof(pkt));
  ThingBot.led(1, res == ESP_OK ? 100 : 0);

  // Bao huong bang LED_2: sang manh = dang day ga
  ThingBot.led(2, abs(throttle));

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 300) {
    lastPrint = millis();
    Serial.printf("ga:%d re:%d\n", throttle, turn);
  }
  delay(40);  // 25 goi/giay
}
