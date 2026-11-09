/*
 * Chuong 19 - Dieu khien robot bang Bluetooth Low Energy (BLE)
 * ThingBot phat BLE ten "ThingBot-BLE" theo chuan Nordic UART Service (NUS).
 *
 * Cach 1 - App "Adafruit Bluefruit Connect" (Android/iOS):
 *   Ket noi "ThingBot-BLE" -> Controller -> Control Pad
 *   Mui ten = di chuyen, nut 1 = coi, nut 2 = doi toc do, nut 3/4 = servo S1
 *
 * Cach 2 - App "Serial Bluetooth Terminal" (Android) hoac "nRF Connect":
 *   Gui chuoi ky tu: F (tien) B (lui) L (trai) R (phai) S (dung) H (coi)
 *                    "M 60 -60" = M1 toc do 60, M2 toc do -60
 */
#include <ThingBot.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define NUS_SERVICE "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUS_RX      "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"  // dien thoai -> robot
#define NUS_TX      "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"  // robot -> dien thoai

BLECharacteristic *txChar;
volatile bool connected = false;

// Lenh nhan duoc (ghi trong callback BLE, doc trong loop)
volatile int cmdLeft = 0, cmdRight = 0;
volatile bool cmdHorn = false;
volatile int cmdServo = -1;
volatile bool cmdToggleSpeed = false;
int speed = 60;

void setDrive(int l, int r) {
  cmdLeft = l;
  cmdRight = r;
}

// Goi tin Bluefruit Control Pad: "!B" + so nut + '1'/'0' + checksum
void handleBluefruit(const String &v) {
  int btn = v[2] - '0';
  bool down = v[3] == '1';
  switch (btn) {
    case 5: setDrive(down ? speed : 0, down ? speed : 0); break;    // len
    case 6: setDrive(down ? -speed : 0, down ? -speed : 0); break;  // xuong
    case 7: setDrive(down ? -speed : 0, down ? speed : 0); break;   // trai
    case 8: setDrive(down ? speed : 0, down ? -speed : 0); break;   // phai
    case 1: cmdHorn = down; break;
    case 2: if (down) cmdToggleSpeed = true; break;
    case 3: if (down) cmdServo = 40; break;
    case 4: if (down) cmdServo = 110; break;
  }
}

// Lenh dang chu
void handleText(String v) {
  v.trim();
  v.toUpperCase();
  if (v == "F") setDrive(speed, speed);
  else if (v == "B") setDrive(-speed, -speed);
  else if (v == "L") setDrive(-speed, speed);
  else if (v == "R") setDrive(speed, -speed);
  else if (v == "S") setDrive(0, 0);
  else if (v == "H") cmdHorn = !cmdHorn;
  else if (v.startsWith("M ")) {
    int sp = v.indexOf(' ', 2);
    if (sp > 0) setDrive(v.substring(2, sp).toInt(), v.substring(sp + 1).toInt());
  }
}

class RxCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *c) override {
    String v = c->getValue();
    if (v.length() >= 4 && v[0] == '!' && v[1] == 'B') handleBluefruit(v);
    else handleText(v);
  }
};

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *s) override { connected = true; }
  void onDisconnect(BLEServer *s) override {
    connected = false;
    setDrive(0, 0);  // mat ket noi -> dung
    cmdHorn = false;
    BLEDevice::startAdvertising();  // cho ket noi lai
  }
};

void sendText(const String &s) {
  if (!connected) return;
  txChar->setValue(s.c_str());
  txChar->notify();
}

void setup() {
  Serial.begin(115200);
  ThingBot.begin();

  BLEDevice::init("ThingBot-BLE");
  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());
  BLEService *service = server->createService(NUS_SERVICE);

  txChar = service->createCharacteristic(NUS_TX, BLECharacteristic::PROPERTY_NOTIFY);
  txChar->addDescriptor(new BLE2902());
  BLECharacteristic *rx = service->createCharacteristic(
      NUS_RX, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
  rx->setCallbacks(new RxCallbacks());

  service->start();
  BLEAdvertising *adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(NUS_SERVICE);
  adv->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("Dang phat BLE: ThingBot-BLE");
  ThingBot.beep();
}

bool wasConnected = false;
unsigned long lastReport = 0;

void loop() {
  // Moi lenh I2C (dong co, servo, coi) chay trong loop() de khong tranh chap bus
  ThingBot.drive(cmdLeft, cmdRight);
  ThingBot.pwm.setPin(TB_BUZZER, cmdHorn ? 2048 : 0);
  if (cmdServo >= 0) {
    ThingBot.servoStart(1, cmdServo, 0.3);
    cmdServo = -1;
  }
  if (cmdToggleSpeed) {
    cmdToggleSpeed = false;
    speed = (speed == 60) ? 100 : (speed == 100 ? 30 : 60);
    sendText("Toc do: " + String(speed) + "\n");
  }
  ThingBot.servoUpdate();

  if (connected != wasConnected) {
    wasConnected = connected;
    ThingBot.led(1, connected ? 100 : 0);
    Serial.println(connected ? "Da ket noi" : "Mat ket noi");
    if (connected) ThingBot.playNote(NOTE_G5, 0.25);
  }

  // Gui khoang cach ve dien thoai moi giay
  if (connected && millis() - lastReport > 1000) {
    lastReport = millis();
    sendText("Khoang cach: " + String(ThingBot.distanceCm(), 0) + " cm\n");
  }
  delay(10);
}
