/*
 * Chuong 14 - Tram thoi tiet mini voi DHT11
 * DHT11 DATA -> IO4. Doc nhiet do & do am moi 2 giay.
 * Qua nong (>= 32 C) -> LED_1 + coi; Qua am (>= 85%) -> LED_2.
 * Moi lan bam SW in ra gia tri nho nhat / lon nhat tu luc bat may.
 * Thu vien: DHT sensor library (Adafruit)
 */
#include <ThingBot.h>
#include <DHT.h>

#define HOT_C   32.0
#define HUMID_P 85.0

DHT dht(TB_DHT, DHT11);
float tMin = 1000, tMax = -1000;
unsigned long lastRead = 0;

void setup() {
  Serial.begin(115200);
  ThingBot.begin();
  dht.begin();
}

void loop() {
  if (millis() - lastRead >= 2000) {
    lastRead = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (isnan(t) || isnan(h)) {
      Serial.println("Loi doc DHT11");
      return;
    }
    tMin = min(tMin, t);
    tMax = max(tMax, t);
    Serial.printf("Nhiet do: %.1f C | Do am: %.0f %%\n", t, h);

    bool hot = t >= HOT_C;
    ThingBot.led(1, hot ? 100 : 0);
    ThingBot.led(2, h >= HUMID_P ? 100 : 0);
    if (hot) ThingBot.beep(150);
  }

  if (ThingBot.buttonClicked()) {
    Serial.printf("Thap nhat: %.1f C, cao nhat: %.1f C\n", tMin, tMax);
  }
}
