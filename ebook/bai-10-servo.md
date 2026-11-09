# Bài 10 – Servo

> **Mục tiêu:** hiểu nguyên lý servo, quay theo góc, quay mượt, nhận lệnh từ Serial.
> **Phần cứng:** servo SG90 hoặc MG90S cắm vào cổng **S1** (dây nâu/đen là GND, cắm đúng chiều).
> **Code:** [`code/10_Servo`](../code/10_Servo/10_Servo.ino)

## 1. Servo hoạt động thế nào?

Servo là một động cơ nhỏ có mạch phản hồi vị trí bên trong. Ta báo cho servo **góc mong muốn** bằng độ rộng xung, lặp lại 50 lần mỗi giây (50 Hz, chu kỳ 20 ms):

```
Độ rộng xung:   0,5 ms        1,5 ms        2,5 ms
                 ┌┐            ┌──┐          ┌────┐
                 ││            │  │          │    │
              ───┘└────     ───┘  └───    ───┘    └──
Góc:              0°            90°          180°
```

Với PCA9685 ở 50 Hz, chu kỳ 20 ms chia làm 4096 phần, nên 0,5 ms ứng với giá trị **102** và 2,5 ms ứng với **512**. Đó là lý do trong thư viện có:

```cpp
#define SERVO_PULSE_MIN 102
#define SERVO_PULSE_MAX 512
pwm.setPWM(kenh, 0, map(goc, 0, 180, SERVO_PULSE_MIN, SERVO_PULSE_MAX));
```

## 2. Ba cách điều khiển servo

| Hàm | Hoạt động |
|---|---|
| `ThingBot.servo(1, 45)` | Quay **ngay lập tức** tới 45° (nhanh hết tốc lực) |
| `ThingBot.servoMove(1, 45, 1.5)` | Quay **mượt** tới 45° trong 1,5 giây, chờ quay xong mới chạy tiếp |
| `ThingBot.servoRelease(1)` | **Thả lỏng**: ngừng phát xung, servo hết giữ lực |

`servoMove()` chia chuyển động thành nhiều bước nhỏ, mỗi bước 20 ms (đúng một chu kỳ xung):

```
góc
180 ┤                ●
    │            ●
    │        ●
    │    ●
  0 ┼●───┬───┬───┬───┬→ thời gian
```

## 3. Nhận lệnh từ Serial Monitor

```cpp
if (Serial.available()) {
  String line = Serial.readStringUntil('\n');
  line.trim();                          // bo khoang trang, ky tu xuong dong
  if (line == "r") ThingBot.servoRelease(1);
  else ThingBot.servoMove(1, line.toInt(), 0.5);
}
```

Gõ `45` rồi Enter: servo quay tới 45°. Gõ `r`: servo thả lỏng.

> **Lưu ý về nguồn:** servo hút dòng mạnh lúc khởi động. Cấp nguồn bằng **pin**, không chỉ bằng USB.
> Một số servo rẻ chỉ quay khoảng 0–170°. Nếu servo **rên** ở hai đầu, hãy giới hạn góc lại (ví dụ 10–170°).

## 4. Thử thách

1. Làm "kim đồng hồ": servo quay 0 → 180° trong 60 giây.
2. Gõ hai số `S G` (servo, góc) để điều khiển nhiều servo, ví dụ `2 135`.
3. Tìm `SERVO_PULSE_MIN` và `SERVO_PULSE_MAX` chính xác cho servo của bạn.

---

[← Bài 9](bai-09-xe-hai-banh.md) · [Mục lục](../README.md) · Tiếp: [Bài 11 – Cánh tay robot →](bai-11-canh-tay-robot.md)
