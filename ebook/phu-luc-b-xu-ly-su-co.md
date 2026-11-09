# Phụ lục B – Xử lý sự cố

## Nạp chương trình

| Hiện tượng | Cách xử lý |
|---|---|
| Không thấy cổng COM | Đổi sang cáp USB có truyền dữ liệu, cài driver CH340/CP210x, thử cổng USB khác |
| `Failed to connect to ESP32-C3` | Giữ **BOOT**, nhấn rồi thả **RESET**, thả **BOOT**, sau đó Upload lại |
| `xxx.h: No such file or directory` | Thiếu thư viện, xem [Cài đặt môi trường](02-cai-dat-moi-truong.md#3-cài-thư-viện) |
| `ThingBot.h: No such file` | Chưa chép `libraries/ThingBot` vào `Documents/Arduino/libraries`, hoặc chưa khởi động lại IDE |
| Serial Monitor trống | **Tools → USB CDC On Boot → Enabled**, baud **115200** |
| `'Mode' was not declared in this scope` dù đã khai báo `enum Mode` | Arduino IDE tự sinh khai báo (prototype) cho mọi hàm và đặt chúng **trước hàm đầu tiên** trong file. Hãy khai báo `enum`/`struct` ở **đầu file**, trước mọi hàm |

## Phần cứng

| Hiện tượng | Nguyên nhân / Cách xử lý |
|---|---|
| LED, động cơ, servo không chạy | Quên gọi `ThingBot.begin()` (hoặc `pwm.begin()`), chưa bật công tắc pin |
| `begin()` trả về `false` | Không thấy PCA9685: mạch hỏng hoặc có thiết bị I2C làm treo bus. Rút các module I2C ngoài ra rồi thử lại |
| Mạch tự khởi động lại khi động cơ chạy | Pin yếu, cần sạc. Tăng tốc từ từ (ramp) |
| Một bánh quay ngược | Đảo 2 dây động cơ hoặc đổi dấu tốc độ |
| Động cơ kêu "ù" mà không quay | Tốc độ quá thấp, tăng lên trên khoảng 30% |
| Servo rung, giật | Nguồn yếu. Đừng phát nhạc trong lúc servo đang chạy (Bài 5) |
| Servo rên ở hai đầu | Giới hạn góc lại (10–170°) hoặc chỉnh `SERVO_PULSE_MIN/MAX` |
| Siêu âm luôn trả về −1 | Cắm nhầm TRIG/ECHO, thiếu nguồn 5 V cho cảm biến, phía trước quá trống |
| DHT11 trả về NaN | Lỏng dây, đọc nhanh hơn 1 lần/giây, đang cắm siêu âm cùng chân IO4 |
| OLED không hiện | Thử địa chỉ `0x3D`. Màn 1.3" là chip SH1106 |
| MPU6050 không tìm thấy | Kiểm tra dây SDA/SCL, địa chỉ `0x68` (chân AD0 nối GND) |

## Không dây

| Hiện tượng | Cách xử lý |
|---|---|
| PS2 báo lỗi 1 | Bật tay cầm trước, đợi đèn bộ thu ngừng nháy, kiểm tra dây cổng PS2 |
| Không thấy Wi-Fi `ThingBot-xxxx` | Đợi 3–5 giây sau khi khởi động. Mật khẩu phải có ít nhất 8 ký tự |
| Trang web không mở được | Một số điện thoại tự chuyển sang 4G khi Wi-Fi không có Internet, hãy tắt dữ liệu di động |
| App không thấy `ThingBot-BLE` | Bật Bluetooth **và** Vị trí (Android), ngắt kết nối cũ trong app |
| ESP-NOW: robot không nhận | Hai bên phải **cùng `ROBOT_ID`, `RF_CHANNEL`** và **struct gói tin giống hệt nhau** |
| ESP-NOW: nhận chập chờn | Đổi `RF_CHANNEL` (1, 6, 11), tránh xa router Wi-Fi, dựng thẳng anten |
| Robot chạy lung tung khi cả lớp cùng chơi | Mỗi nhóm đặt `ROBOT_ID` khác nhau |

## Công cụ gỡ lỗi

- **`Serial.printf("x=%d\n", x);`** để in giá trị biến. Đây là công cụ gỡ lỗi số 1.
- **Serial Plotter** để vẽ đồ thị cảm biến (in theo dạng `ten:gia_tri`).
- **Quét I2C** để xem có những thiết bị nào trên bus:

```cpp
#include <Wire.h>
void setup() {
  Serial.begin(115200); delay(1500); Wire.begin();
  for (byte a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) Serial.printf("Tim thay 0x%02X\n", a);
  }
}
void loop() {}
```

Thông thường bạn sẽ thấy `0x40` (PCA9685), `0x3C` (OLED) và `0x68` (MPU6050).

---

[← Phụ lục A](phu-luc-a-thu-vien-thingbot.md) · [Mục lục](../README.md)
