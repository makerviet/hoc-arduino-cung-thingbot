# Bài 13 – Màn hình OLED

> **Mục tiêu:** hiển thị chữ, số, hình vẽ. Làm "khuôn mặt" robot.
> **Phần cứng:** OLED 0.96" SSD1306 128×64, cắm cổng **I2C** (địa chỉ `0x3C`), và cảm biến siêu âm.
> **Thư viện:** Adafruit SSD1306, Adafruit GFX Library.
> **Code:** [`code/13_OLED`](../code/13_OLED/13_OLED.ino)

## 1. Hệ tọa độ

```
(0,0) ───────────── x ──────────→ (127,0)
  │
  y        Màn hình 128 × 64 điểm ảnh
  │
  ↓
(0,63)                            (127,63)
```

Gốc (0, 0) nằm ở **góc trên bên trái**. Trục y hướng **xuống dưới**.

## 2. Vẽ bằng bộ đệm

Thư viện vẽ mọi thứ vào **bộ đệm** trong RAM trước. Chỉ khi gọi `display()` thì nội dung mới được gửi lên màn hình. Nhờ vậy hình không bị nháy:

```cpp
oled.clearDisplay();          // 1. xoa bo dem
oled.setCursor(0, 0);         // 2. ve...
oled.print("Xin chao");
oled.drawCircle(64, 40, 10, SSD1306_WHITE);
oled.display();               // 3. day len man hinh
```

| Hàm | Vẽ |
|---|---|
| `setTextSize(n)` | Cỡ chữ (1 = cao 8 px, 2 = cao 16 px …) |
| `setCursor(x, y)` / `print()` / `println()` | Chữ, số |
| `drawLine(x0, y0, x1, y1, màu)` | Đường thẳng |
| `drawRect / fillRect(x, y, w, h, màu)` | Hình chữ nhật rỗng / đặc |
| `drawCircle / fillCircle(x, y, r, màu)` | Hình tròn |
| `drawRoundRect / fillRoundRect(x, y, w, h, r, màu)` | Chữ nhật bo góc |

## 3. Bảng đo khoảng cách có khuôn mặt

- Số khoảng cách hiển thị ở cỡ chữ 2.
- Thanh ngang ở dưới dài ra khi vật đến gần.
- Robot "cười" khi vật ở xa và "hoảng hốt" khi vật gần hơn 20 cm.

```cpp
int bar = map(constrain((int)d, 0, 100), 0, 100, 70, 0);
oled.drawRect(0, 40, 72, 10, SSD1306_WHITE);
oled.fillRect(1, 41, bar, 8, SSD1306_WHITE);
drawFace(d < 20);
```

> `ThingBot.begin()` đã gọi `Wire.begin()`, nên OLED và PCA9685 dùng chung bus I2C mà không cần làm gì thêm.
> Font mặc định **không có dấu tiếng Việt**.
> Màn **1.3" SH1106**: dùng thư viện *Adafruit SH110X* và lớp `Adafruit_SH1106G`.

## 4. Thử thách

1. Vẽ thêm đôi mắt **chớp** mỗi 3 giây (dùng `millis()`).
2. Hiển thị đồng hồ phút:giây tính từ lúc khởi động.
3. Vẽ đồ thị khoảng cách chạy ngang màn hình: lưu 128 giá trị gần nhất trong một mảng.

---

[← Bài 12](bai-12-sieu-am.md) · [Mục lục](../README.md) · Tiếp: [Bài 14 – DHT11 →](bai-14-dht11.md)
