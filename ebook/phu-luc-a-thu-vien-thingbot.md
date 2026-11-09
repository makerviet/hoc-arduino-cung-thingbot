# Phụ lục A – Tham khảo thư viện ThingBot

```cpp
#include <ThingBot.h>   // doi tuong toan cuc: ThingBot
```

Mã nguồn: [`libraries/ThingBot/src`](../libraries/ThingBot/src)

## Khởi động

| Hàm | Mô tả |
|---|---|
| `bool begin()` | Khởi động I2C, PCA9685 (50 Hz), nút SW. Tắt hết LED, còi và động cơ. Trả về `false` nếu không thấy PCA9685 ở địa chỉ `0x40`. |

## LED và nút

| Hàm | Mô tả |
|---|---|
| `void led(which, percent)` | `which` = 1 hoặc 2, `percent` = 0..100 |
| `bool buttonHeld()` | `true` khi đang giữ SW |
| `bool buttonClicked()` | `true` **đúng một lần** cho mỗi lần bấm (đã chống dội 25 ms). Phải gọi thường xuyên trong `loop()`. |

## Còi và âm nhạc

| Hàm | Mô tả |
|---|---|
| `void beep(ms = 80)` | Bíp ngắn ở tần số 50 Hz (tiếng trầm) |
| `void setTempo(bpm)` | Đặt nhịp, mặc định 120 |
| `void playNote(hz, beats)` | Phát nốt `hz` trong `beats` phách. `hz = 0` (`NOTE_REST`) là nghỉ. Hàm chặn cho tới khi phát xong. |

Hằng số nốt: `NOTE_C4` … `NOTE_B4`, `NOTE_CS4` (Đô thăng)…, `NOTE_C5` … `NOTE_B5`, `NOTE_C6`.

> `playNote` tạm đổi tần số của **toàn bộ** PCA9685, nên servo sẽ chạy sai trong lúc phát nốt.

## Động cơ DC

| Hàm | Mô tả |
|---|---|
| `void motor(m, speed)` | `m` = 1..4, `speed` = −100 (lùi hết tốc) … 0 (dừng) … 100 (tiến hết tốc) |
| `void drive(left, right)` | Xe 2 bánh: M1 = trái, M2 = phải |
| `void stop()` | Dừng cả 4 động cơ |

## Servo

| Hàm | Mô tả |
|---|---|
| `void servo(s, deg)` | `s` = 1..5, quay ngay tới `deg` (0..180) |
| `void servoMove(s, deg, seconds)` | Quay mượt, **chờ** tới khi xong |
| `void servoStart(s, deg, seconds)` | Bắt đầu quay mượt rồi chạy tiếp ngay |
| `bool servoUpdate()` | Gọi liên tục trong `loop()` khi dùng `servoStart`. Trả về `true` nếu còn servo đang quay. |
| `void servoWait()` | Chờ mọi servo quay xong |
| `void servoRelease(s)` | Ngừng phát xung, servo thả lỏng |
| `int servoAngle(s)` | Góc hiện tại (do thư viện ghi nhớ) |

## Cảm biến siêu âm

| Hàm | Mô tả |
|---|---|
| `void ultrasonicPins(trig, echo)` | Đổi chân (mặc định TRIG = IO0, ECHO = IO4) |
| `float distanceCm()` | Khoảng cách tính bằng cm, `-1` nếu không đo được. Chặn tối đa khoảng 30 ms. |

## Truy cập trực tiếp

`ThingBot.pwm` là đối tượng `Adafruit_PWMServoDriver`, dùng khi cần điều khiển kênh PCA9685 theo cách riêng:

```cpp
ThingBot.pwm.setPin(TB_BUZZER, 2048);   // coi keu lien tuc
```

## Hằng số chân

| Hằng số | Giá trị | | Hằng số | Giá trị |
|---|---|---|---|---|
| `TB_M1_A / TB_M1_B` | 2 / 3 | | `TB_SW` | IO3 |
| `TB_M2_A / TB_M2_B` | 4 / 5 | | `TB_TRIG` | IO0 |
| `TB_M3_A / TB_M3_B` | 7 / 6 | | `TB_ECHO`, `TB_DHT` | IO4 |
| `TB_M4_A / TB_M4_B` | 1 / 0 | | `TB_PS2_DAT` | IO7 |
| `TB_SERVO_1 … TB_SERVO_5` | 12 … 8 | | `TB_PS2_CMD` | IO2 |
| `TB_BUZZER` | 14 | | `TB_PS2_SEL` | IO10 |
| `TB_LED_1 / TB_LED_2` | 15 / 13 | | `TB_PS2_CLK` | IO6 |

---

[Mục lục](../README.md) · [Phụ lục B – Xử lý sự cố →](phu-luc-b-xu-ly-su-co.md)
