# Cài đặt môi trường lập trình

## 1. Cài Arduino IDE

Tải **Arduino IDE 2.x** tại <https://www.arduino.cc/en/software> rồi cài như phần mềm thông thường.

## 2. Thêm board ESP32

1. Mở **File → Preferences**.
2. Ở ô **Additional boards manager URLs**, dán:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Mở **Tools → Board → Boards Manager**, gõ `esp32`, cài gói **esp32 by Espressif Systems** (bản **3.x**).

## 3. Cài thư viện

Mở **Sketch → Include Library → Manage Libraries…** rồi cài:

| Thư viện | Dùng từ bài |
|---|---|
| **Adafruit PWM Servo Driver Library** (chọn *Install all* để cài kèm Adafruit BusIO) | 2 |
| **Adafruit SSD1306** và **Adafruit GFX Library** | 13, 21 |
| **DHT sensor library** (Adafruit) | 14 |
| **Adafruit MPU6050** và **Adafruit Unified Sensor** | 15, 20 |
| **PS2X_lib** (không có trong Library Manager: tải `.zip` trên GitHub rồi chọn **Sketch → Include Library → Add .ZIP Library**) | 17 |

### Thư viện ThingBot (của sách này)

Từ Bài 7 trở đi, sách dùng thư viện **ThingBot** cho code ngắn gọn. Cách cài:

- **Cách 1:** chép thư mục [`libraries/ThingBot`](../libraries/ThingBot) vào `Documents/Arduino/libraries/`, rồi khởi động lại Arduino IDE.
- **Cách 2:** nén thư mục `libraries/ThingBot` thành `ThingBot.zip`, rồi chọn **Sketch → Include Library → Add .ZIP Library**.

## 4. Chọn board và cổng

| Mục trong menu **Tools** | Giá trị |
|---|---|
| Board | **esp32 → ESP32C3 Dev Module** |
| USB CDC On Boot | **Enabled** ← bắt buộc, nếu không Serial Monitor sẽ trống |
| Upload Speed | 921600 |
| Port | Cổng COM xuất hiện khi cắm ThingBot |

> **Không thấy cổng COM?** Thử dây cáp USB khác, vì nhiều dây chỉ sạc được mà không truyền dữ liệu. Trên Windows có thể cần cài driver CH340 hoặc CP210x, tùy phiên bản mạch.

## 5. Nạp chương trình đầu tiên

1. Mở [`code/01_XinChao/01_XinChao.ino`](../code/01_XinChao/01_XinChao.ino).
2. Bấm nút **Upload** (mũi tên →).
3. Khi thấy dòng `Hard resetting via RTS pin...`, việc nạp đã xong.
4. Mở **Tools → Serial Monitor**, chọn **115200 baud**.

> **Nạp lỗi** (`Failed to connect`, `No serial data received`): **giữ nút BOOT**, nhấn rồi thả **RESET**, thả **BOOT**, sau đó bấm Upload lại.

## 6. Dùng arduino-cli (tùy chọn, cho người dùng nâng cao)

```bash
arduino-cli core install esp32:esp32
arduino-cli compile -b esp32:esp32:esp32c3:CDCOnBoot=cdc --library libraries/ThingBot code/07_ThuVienThingBot
arduino-cli upload  -b esp32:esp32:esp32c3:CDCOnBoot=cdc -p COM5 code/07_ThuVienThingBot
```

---

[← Giới thiệu ThingBot](01-gioi-thieu-thingbot.md) · [Mục lục](../README.md) · Tiếp: [Bài 1 – Xin chào →](bai-01-xin-chao.md)
