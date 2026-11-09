/*
 * Chuong 21 - DU AN CUOI: Robot dieu khien RF hoan chinh
 * -----------------------------------------------------
 * Dung chung tay dieu khien cua Chuong 20a (khong can sua).
 *
 * Tinh nang:
 *  - 2 che do: MANUAL (lai bang tay dieu khien RF) va AUTO (tu tranh vat can)
 *    Bam SW tren tay dieu khien HOAC tren robot de doi che do.
 *  - Ho tro chong dam: o MANUAL, vat can < 15 cm thi khong cho tien them
 *  - Failsafe: mat song 300 ms -> dung (che do MANUAL)
 *  - Tang/giam toc muot (ramp) de xe khong giat, khong sut ap
 *  - Man hinh OLED (neu co): che do, song RF, khoang cach, toc do 2 banh
 *  - Giu SW tren tay dieu khien = coi
 *
 * Phan cung robot: M1 trai, M2 phai, sieu am TRIG IO0 / ECHO IO4,
 *                  OLED SSD1306 0x3C (tuy chon)
 * Thu vien: ThingBot, Adafruit SSD1306, Adafruit GFX
 */
#include <ThingBot.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= CAU HINH =================
#define ROBOT_ID      1
#define RF_CHANNEL    1
#define FAILSAFE_MS   300
#define GUARD_CM      15    // chong dam o MANUAL
#define AUTO_SPEED    55
#define AUTO_TURN     55
#define AUTO_OBST_CM  25
#define RAMP_STEP     8     // % moi chu ky 20 ms

// ================= GOI TIN RF =================
struct __attribute__((packed)) RemotePacket {
  uint16_t magic;
  uint8_t robotId;
  int8_t throttle;
  int8_t turn;
  uint8_t buttons;
  uint8_t clickCount;
};

// Khai bao kieu TRUOC ham dau tien: Arduino IDE tu sinh "prototype" cho moi ham
// va dat chung ngay truoc ham dau tien trong file.
enum Mode { MANUAL, AUTO };
enum AutoState { A_FORWARD, A_BACK, A_TURN };

portMUX_TYPE rxMux = portMUX_INITIALIZER_UNLOCKED;
RemotePacket rxPkt = {};
volatile unsigned long rxMs = 0;
volatile uint32_t rxCount = 0;

void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != sizeof(RemotePacket)) return;
  RemotePacket p;
  memcpy(&p, data, sizeof(p));
  if (p.magic != 0x7B07 || p.robotId != ROBOT_ID) return;
  portENTER_CRITICAL(&rxMux);
  rxPkt = p;
  rxMs = millis();
  rxCount++;
  portEXIT_CRITICAL(&rxMux);
}

// ================= TRANG THAI =================
Mode mode = MANUAL;
AutoState autoState = A_FORWARD;
unsigned long autoStart = 0, autoTurnMs = 500;
int autoTurnDir = 1;

int targetL = 0, targetR = 0;  // toc do mong muon
int curL = 0, curR = 0;        // toc do thuc te (sau ramp)
float distance = 999;
bool linkOk = false;
int rxRate = 0;
uint8_t lastClick = 0;
bool clickSynced = false;

Adafruit_SSD1306 oled(128, 64, &Wire);
bool hasOled = false;

// ================= HAM TIEN ICH =================
int ramp(int cur, int target) {
  if (cur < target) return min(cur + RAMP_STEP, target);
  if (cur > target) return max(cur - RAMP_STEP, target);
  return cur;
}

void setMode(Mode m) {
  mode = m;
  targetL = targetR = 0;
  if (m == AUTO) {
    autoState = A_FORWARD;
    autoStart = millis();
    ThingBot.playNote(NOTE_C5, 0.25);
    ThingBot.playNote(NOTE_G5, 0.25);
  } else {
    ThingBot.playNote(NOTE_G5, 0.25);
    ThingBot.playNote(NOTE_C5, 0.25);
  }
  Serial.println(m == AUTO ? "Che do: AUTO" : "Che do: MANUAL");
}

void autoEnter(AutoState s) {
  autoState = s;
  autoStart = millis();
  if (s == A_TURN) {
    autoTurnDir = random(2) ? 1 : -1;
    autoTurnMs = random(300, 700);
  }
}

// ================= HANH VI =================
void updateManual(const RemotePacket &p) {
  if (!linkOk) {
    targetL = targetR = 0;
    return;
  }
  int throttle = p.throttle;
  if (throttle > 0 && distance < GUARD_CM) throttle = 0;  // chong dam
  targetL = constrain(throttle + p.turn, -100, 100);
  targetR = constrain(throttle - p.turn, -100, 100);
}

void updateAuto() {
  unsigned long t = millis() - autoStart;
  switch (autoState) {
    case A_FORWARD:
      targetL = targetR = AUTO_SPEED;
      if (distance < AUTO_OBST_CM) autoEnter(A_BACK);
      break;
    case A_BACK:
      targetL = targetR = -AUTO_SPEED;
      if (t > 400) autoEnter(A_TURN);
      break;
    case A_TURN:
      targetL = AUTO_TURN * autoTurnDir;
      targetR = -AUTO_TURN * autoTurnDir;
      if (t > autoTurnMs) autoEnter(A_FORWARD);
      break;
  }
}

// ================= MAN HINH =================
void drawBar(int x, int y, int value) {  // value -100..100, thanh doc
  oled.drawRect(x, y, 10, 40, SSD1306_WHITE);
  int h = abs(value) * 19 / 100;
  if (value >= 0) oled.fillRect(x + 1, y + 20 - h, 8, h, SSD1306_WHITE);
  else oled.fillRect(x + 1, y + 20, 8, h, SSD1306_WHITE);
}

void drawScreen() {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(0, 0);
  oled.print(mode == AUTO ? "AUTO" : "MANUAL");

  oled.setTextSize(1);
  oled.setCursor(0, 22);
  oled.printf("RF : %s %d/s", linkOk ? "OK" : "--", rxRate);
  oled.setCursor(0, 34);
  if (distance >= 999) oled.print("Cach: ---");
  else oled.printf("Cach: %d cm", (int)distance);
  oled.setCursor(0, 46);
  oled.printf("ID %d  CH %d", ROBOT_ID, RF_CHANNEL);

  drawBar(100, 20, curL);
  drawBar(114, 20, curR);
  oled.setCursor(101, 4);
  oled.print("L R");
  oled.display();
}

// ================= SETUP / LOOP =================
void setup() {
  Serial.begin(115200);
  ThingBot.begin();
  Wire.setClock(400000);  // I2C nhanh de OLED khong lam cham vong lap
  randomSeed(esp_random());

  Wire.beginTransmission(0x3C);
  hasOled = (Wire.endTransmission() == 0) && oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (hasOled) {
    oled.setTextColor(SSD1306_WHITE);
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setCursor(10, 24);
    oled.print("ThingBot");
    oled.display();
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(RF_CHANNEL, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() == ESP_OK) esp_now_register_recv_cb(onReceive);
  else Serial.println("Loi ESP-NOW");

  // Nhac khoi dong
  ThingBot.setTempo(240);
  ThingBot.playNote(NOTE_C5, 1);
  ThingBot.playNote(NOTE_E5, 1);
  ThingBot.playNote(NOTE_G5, 1);
  ThingBot.playNote(NOTE_C6, 2);
  setMode(MANUAL);
}

void loop() {
  static unsigned long lastTick = 0, lastDist = 0, lastScreen = 0, lastRate = 0;
  static uint32_t lastCount = 0;
  unsigned long now = millis();

  // 1) Lay goi tin RF moi nhat
  RemotePacket p;
  unsigned long pktMs;
  portENTER_CRITICAL(&rxMux);
  p = rxPkt;
  pktMs = rxMs;
  portEXIT_CRITICAL(&rxMux);
  linkOk = pktMs != 0 && now - pktMs < FAILSAFE_MS;

  // 2) Doi che do: bam SW tren robot, hoac clickCount tren tay dieu khien thay doi
  bool toggle = ThingBot.buttonClicked();
  if (linkOk) {
    if (!clickSynced) {  // goi tin dau tien: chi ghi nho, khong doi che do
      lastClick = p.clickCount;
      clickSynced = true;
    } else if (p.clickCount != lastClick) {
      lastClick = p.clickCount;
      toggle = true;
    }
  }
  if (toggle) setMode(mode == MANUAL ? AUTO : MANUAL);

  // 3) Do khoang cach moi 50 ms
  if (now - lastDist >= 50) {
    lastDist = now;
    float d = ThingBot.distanceCm();
    distance = d < 0 ? 999 : d;
  }

  // 4) Tinh toc do mong muon va dieu khien dong co moi 20 ms
  if (now - lastTick >= 20) {
    lastTick = now;
    if (mode == MANUAL) updateManual(p);
    else updateAuto();
    curL = ramp(curL, targetL);
    curR = ramp(curR, targetR);
    ThingBot.drive(curL, curR);

    bool horn = linkOk && (p.buttons & 1);
    ThingBot.pwm.setPin(TB_BUZZER, horn ? 2048 : 0);
    ThingBot.led(1, mode == AUTO ? 100 : (linkOk ? 30 : 0));
    ThingBot.led(2, (mode == MANUAL && !linkOk && (now / 250) % 2) ? 100 : 0);
  }

  // 5) Thong ke so goi/giay
  if (now - lastRate >= 1000) {
    lastRate = now;
    rxRate = rxCount - lastCount;
    lastCount = rxCount;
  }

  // 6) Cap nhat man hinh 5 lan/giay
  if (hasOled && now - lastScreen >= 200) {
    lastScreen = now;
    drawScreen();
  }
}
