/*
 * Chuong 13 - Man hinh OLED SSD1306 128x64 (I2C 0x3C)
 * Bang dieu khien mini: hien khoang cach sieu am bang so va thanh ngang,
 * ve "khuon mat" robot doi bieu cam theo khoang cach.
 * Thu vien: Adafruit SSD1306, Adafruit GFX Library
 */
#include <ThingBot.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 oled(128, 64, &Wire);

void drawFace(bool scared) {
  // mat
  if (scared) {
    oled.drawCircle(88, 22, 7, SSD1306_WHITE);
    oled.drawCircle(112, 22, 7, SSD1306_WHITE);
    oled.fillCircle(88, 22, 2, SSD1306_WHITE);
    oled.fillCircle(112, 22, 2, SSD1306_WHITE);
    oled.drawCircle(100, 44, 6, SSD1306_WHITE);  // mieng "O"
  } else {
    oled.fillRoundRect(81, 16, 14, 12, 4, SSD1306_WHITE);
    oled.fillRoundRect(105, 16, 14, 12, 4, SSD1306_WHITE);
    oled.drawLine(90, 42, 100, 48, SSD1306_WHITE);  // mieng cuoi
    oled.drawLine(100, 48, 110, 42, SSD1306_WHITE);
  }
}

void setup() {
  Serial.begin(115200);
  ThingBot.begin();  // da goi Wire.begin()

  // oled.begin() chi bao loi khi thieu RAM, KHONG phat hien duoc man hinh chua cam.
  // Vi vay hoi thu dia chi 0x3C tren bus I2C truoc.
  Wire.beginTransmission(0x3C);
  bool found = (Wire.endTransmission() == 0);
  if (!found || !oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Khong tim thay OLED!");
    while (true) delay(1000);
  }
  oled.setTextColor(SSD1306_WHITE);
}

void loop() {
  float d = ThingBot.distanceCm();
  if (d < 0) d = 999;

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.print("Khoang cach");

  oled.setTextSize(2);
  oled.setCursor(0, 16);
  if (d >= 999) oled.print("---");
  else oled.print((int)d);
  oled.setTextSize(1);
  oled.print(" cm");

  // thanh ngang: day = gan
  int bar = map(constrain((int)d, 0, 100), 0, 100, 70, 0);
  oled.drawRect(0, 40, 72, 10, SSD1306_WHITE);
  oled.fillRect(1, 41, bar, 8, SSD1306_WHITE);

  drawFace(d < 20);
  oled.display();
  delay(100);
}
