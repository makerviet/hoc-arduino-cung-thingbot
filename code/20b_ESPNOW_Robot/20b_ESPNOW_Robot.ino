/*
 * Chuong 20b - ROBOT nhan lenh RF (ESP-NOW)
 * Nap vao ThingBot thu HAI (robot xe: M1 trai, M2 phai).
 * Nhan goi tin tu tay dieu khien (Chuong 20a) va chay theo.
 * An toan: mat song qua 300 ms -> dung ngay, LED_2 nhay.
 */
#include <ThingBot.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define ROBOT_ID    1   // PHAI GIONG tay dieu khien
#define RF_CHANNEL  1
#define FAILSAFE_MS 300

struct __attribute__((packed)) RemotePacket {
  uint16_t magic;
  uint8_t robotId;
  int8_t throttle;
  int8_t turn;
  uint8_t buttons;
  uint8_t clickCount;
};

// Du lieu dung chung giua callback (chay o tac vu Wi-Fi) va loop()
portMUX_TYPE rxMux = portMUX_INITIALIZER_UNLOCKED;
RemotePacket lastPkt = {};
volatile unsigned long lastRxMs = 0;
volatile uint32_t rxCount = 0;

void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != sizeof(RemotePacket)) return;
  RemotePacket p;
  memcpy(&p, data, sizeof(p));
  if (p.magic != 0x7B07 || p.robotId != ROBOT_ID) return;  // khong phai cua minh
  portENTER_CRITICAL(&rxMux);
  lastPkt = p;
  lastRxMs = millis();
  rxCount++;
  portEXIT_CRITICAL(&rxMux);
}

void setup() {
  Serial.begin(115200);
  ThingBot.begin();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(RF_CHANNEL, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Loi khoi dong ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(onReceive);
  Serial.printf("Robot san sang. ROBOT_ID=%d, kenh %d\n", ROBOT_ID, RF_CHANNEL);
  ThingBot.beep();
}

void loop() {
  RemotePacket p;
  unsigned long rxMs;
  portENTER_CRITICAL(&rxMux);
  p = lastPkt;
  rxMs = lastRxMs;
  portEXIT_CRITICAL(&rxMux);

  bool linkOk = rxMs != 0 && millis() - rxMs < FAILSAFE_MS;

  if (linkOk) {
    int left = constrain(p.throttle + p.turn, -100, 100);
    int right = constrain(p.throttle - p.turn, -100, 100);
    ThingBot.drive(left, right);
    ThingBot.pwm.setPin(TB_BUZZER, (p.buttons & 1) ? 2048 : 0);
    ThingBot.led(1, 100);
    ThingBot.led(2, 0);
  } else {
    ThingBot.stop();
    ThingBot.pwm.setPin(TB_BUZZER, 0);
    ThingBot.led(1, 0);
    ThingBot.led(2, (millis() / 250) % 2 ? 100 : 0);  // nhay = mat song
  }

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    lastPrint = millis();
    static uint32_t lastCount = 0;
    Serial.printf("Song: %s | %u goi/giay | ga %d re %d\n",
                  linkOk ? "OK" : "MAT", rxCount - lastCount, p.throttle, p.turn);
    lastCount = rxCount;
  }
  delay(10);
}
