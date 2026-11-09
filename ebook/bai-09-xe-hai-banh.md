# Bài 9 – Xe hai bánh

> **Mục tiêu:** điều khiển xe vi sai (2 bánh chủ động), viết hàm di chuyển, căn chỉnh góc rẽ.
> **Phần cứng:** khung xe 2 bánh, **M1 = bánh trái**, **M2 = bánh phải**.
> **Code:** [`code/09_XeHaiBanh`](../code/09_XeHaiBanh/09_XeHaiBanh.ino)

## 1. Xe vi sai

Xe không có vô lăng. Nó rẽ bằng cách cho **hai bánh quay khác nhau**:

| Động tác | M1 (trái) | M2 (phải) | Hàm |
|---|---|---|---|
| Đi thẳng | + | + | `ThingBot.drive(60, 60)` |
| Lùi | − | − | `ThingBot.drive(-60, -60)` |
| Xoay trái tại chỗ | − | + | `ThingBot.drive(-60, 60)` |
| Xoay phải tại chỗ | + | − | `ThingBot.drive(60, -60)` |
| Rẽ cung phải | + nhanh | + chậm | `ThingBot.drive(70, 30)` |

> **Bước đầu tiên:** chạy `ThingBot.drive(60, 60)`. Nếu một bánh quay **lùi**, hãy đảo 2 dây của động cơ đó, hoặc đổi dấu tốc độ trong code.

## 2. Các hàm di chuyển

```cpp
#define SPEED       60
#define TURN_90_MS  420    // CAN CHINH

void forward(int ms)   { ThingBot.drive(SPEED, SPEED);   delay(ms); ThingBot.stop(); }
void turnRight(int ms) { ThingBot.drive(SPEED, -SPEED);  delay(ms); ThingBot.stop(); }

void square() {
  for (int i = 0; i < 4; i++) {
    forward(SIDE_MS);
    turnRight(TURN_90_MS);
  }
}
```

Chương trình chính đọc như một câu chuyện: *đi thẳng, rẽ phải, lặp 4 lần*. Đặt tên hàm rõ ràng giúp code **tự giải thích**.

## 3. Căn chỉnh góc rẽ

Xe không có cảm biến góc, nên ta rẽ **theo thời gian**. Thời gian xoay đủ 90° phụ thuộc vào pin, mặt sàn và bánh xe:

1. Đặt `TURN_90_MS = 400`, nạp code rồi bấm SW.
2. Nếu xe xoay **chưa đủ** góc vuông thì tăng thêm 20–30 ms. Nếu **quá** thì giảm bớt.
3. Lặp lại cho tới khi xe đi được hình vuông khép kín.

> Pin yếu thì xe xoay chậm hơn, nên thời gian rẽ lệch đi. Đây là giới hạn của điều khiển **vòng hở** (không có phản hồi). Ở Bài 15, cảm biến MPU6050 (con quay hồi chuyển) sẽ cho phép rẽ chính xác theo góc thật.

## 4. Thử thách

1. Cho xe đi hình **tam giác đều** (mỗi góc rẽ 120°).
2. Đi hình số **8** bằng cách rẽ cung tròn (một bánh nhanh, một bánh chậm).
3. Viết hàm `turnDegrees(int deg)` tính thời gian rẽ theo `TURN_90_MS * deg / 90`.

---

[← Bài 8](bai-08-dong-co.md) · [Mục lục](../README.md) · Tiếp: [Bài 10 – Servo →](bai-10-servo.md)
