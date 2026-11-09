# Bài 14 – Trạm thời tiết mini với DHT11

> **Mục tiêu:** đọc cảm biến số một dây, xử lý lỗi `NaN`, theo dõi giá trị nhỏ nhất và lớn nhất.
> **Phần cứng:** DHT11, chân DATA nối **IO4**.
> **Thư viện:** DHT sensor library (Adafruit).
> **Code:** [`code/14_DHT11`](../code/14_DHT11/14_DHT11.ino)

## 1. DHT11

| Thông số | DHT11 | DHT22 |
|---|---|---|
| Nhiệt độ | 0–50 °C, ±2 °C | −40–80 °C, ±0,5 °C |
| Độ ẩm | 20–90 %, ±5 % | 0–100 %, ±2 % |
| Tốc độ đọc | 1 lần/giây | 1 lần/2 giây |

Muốn dùng DHT22 thì chỉ cần đổi `DHT dht(TB_DHT, DHT11);` thành `DHT22`.

> IO4 cũng là chân **ECHO** của cảm biến siêu âm. Hai cảm biến này không cắm cùng lúc được.

## 2. Đọc và kiểm tra lỗi

```cpp
float t = dht.readTemperature();
float h = dht.readHumidity();
if (isnan(t) || isnan(h)) {
  Serial.println("Loi doc DHT11");
  return;                    // bo qua lan nay
}
```

`NaN` (Not a Number) là giá trị "không phải số" mà thư viện trả về khi đọc lỗi, ví dụ do lỏng dây hoặc đọc quá nhanh. `isnan()` dùng để kiểm tra trường hợp này. **Luôn kiểm tra** trước khi dùng giá trị đọc được.

## 3. Đọc định kỳ mà không chặn chương trình

Áp dụng kỹ thuật `millis()` của Bài 6: DHT11 được đọc mỗi 2 giây, trong khi nút SW vẫn phản hồi tức thì.

```cpp
if (millis() - lastRead >= 2000) { ... doc DHT ... }
if (ThingBot.buttonClicked())    { ... in min/max ... }
```

## 4. Giá trị nhỏ nhất và lớn nhất

```cpp
float tMin = 1000, tMax = -1000;   // khoi tao "nguoc"
tMin = min(tMin, t);
tMax = max(tMax, t);
```

Mẹo: khởi tạo `tMin` bằng một số rất **lớn** để lần đọc đầu tiên chắc chắn nhỏ hơn nó. `tMax` thì làm ngược lại.

## 5. Thử thách

1. Hiển thị nhiệt độ và độ ẩm lên OLED (kết hợp Bài 13).
2. Tính **nhiệt độ trung bình** của 10 lần đo gần nhất.
3. Làm quạt tự động: nhiệt độ ≥ 30 °C thì động cơ M1 (gắn cánh quạt) quay, tốc độ tăng theo nhiệt độ.

---

[← Bài 13](bai-13-oled.md) · [Mục lục](../README.md) · Tiếp: [Bài 15 – MPU6050 →](bai-15-mpu6050.md)
