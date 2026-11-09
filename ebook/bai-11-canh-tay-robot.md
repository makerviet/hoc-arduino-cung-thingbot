# Bài 11 – Cánh tay robot 3 khớp

> **Mục tiêu:** điều khiển nhiều servo **song song**, lưu "tư thế" bằng mảng, chia chương trình thành hàm.
> **Phần cứng:** S1 = đế xoay, S2 = khuỷu tay, S3 = kẹp.
> **Code:** [`code/11_CanhTayRobot`](../code/11_CanhTayRobot/11_CanhTayRobot.ino)

## 1. Tuần tự hay song song?

Nếu viết:

```cpp
ThingBot.servoMove(BASE, 160, 1.0);   // xong moi chay dong duoi
ThingBot.servoMove(ELBOW, 130, 1.0);
```

thì cánh tay sẽ xoay đế xong rồi mới hạ khuỷu, mất 2 giây và trông giống robot đời cũ. Muốn **hai khớp cùng chạy**, ta dùng bộ ba hàm:

| Hàm | Ý nghĩa |
|---|---|
| `servoStart(s, góc, giây)` | **Bắt đầu** chuyển động rồi chạy tiếp ngay, không chờ |
| `servoUpdate()` | Đẩy mọi servo đang chạy tới vị trí đúng với thời điểm hiện tại |
| `servoWait()` | Gọi `servoUpdate()` liên tục cho tới khi mọi servo tới đích |

```cpp
void goTo(const int pose[2], float sec) {
  ThingBot.servoStart(BASE, pose[0], sec);
  ThingBot.servoStart(ELBOW, pose[1], sec);
  ThingBot.servoWait();          // ca hai cung toi dich sau `sec` giay
}
```

## 2. Lưu tư thế bằng mảng

```cpp
const int HOME[2]  = {90, 60};    // {goc de, goc khuyu}
const int LEFT[2]  = {160, 130};
const int RIGHT[2] = {20, 130};
```

Muốn đổi vị trí gắp thì chỉ cần sửa số trong mảng, không phải động vào logic.

## 3. Chuỗi gắp – thả

```mermaid
flowchart LR
  A[Mở kẹp] --> B[Tới TRÁI] --> C[Đóng kẹp] --> D[Nhấc lên]
  D --> E[Xoay sang PHẢI] --> F[Hạ xuống] --> G[Mở kẹp] --> H[Về HOME]
```

```cpp
void pickAndPlace() {
  grip(false);
  goTo(LEFT, 1.0);
  grip(true);
  lift();
  ThingBot.servoMove(BASE, RIGHT[0], 1.2);
  goTo(RIGHT, 0.6);
  grip(false);
  goTo(HOME, 1.0);
}
```

## 4. Căn chỉnh

Mỗi cánh tay lắp ráp hơi khác nhau. Hãy dùng chương trình Bài 10 (gõ góc từ Serial) để tìm góc đúng cho từng tư thế, rồi điền vào mảng.

> Không để servo kẹp quá chặt (góc `GRIP_CLOSE` quá lớn), vì servo sẽ rên, nóng lên và hao pin. Giảm góc kẹp tới khi vừa giữ chắc vật.

## 5. Thử thách

1. Thêm tư thế `CENTER` và cho cánh tay gắp vật từ TRÁI sang GIỮA rồi sang PHẢI.
2. Chế độ **dạy học**: gõ góc từ Serial để đưa tay tới vị trí, bấm SW để *ghi nhớ* tư thế vào mảng. Ghi xong 5 tư thế thì cánh tay tự phát lại.
3. Dùng `servoStart()` + `servoUpdate()` trong `loop()` (không gọi `servoWait()`) để cánh tay vừa chuyển động vừa nháy LED theo `millis()`.

---

[← Bài 10](bai-10-servo.md) · [Mục lục](../README.md) · Tiếp: [Phần 3 – Bài 12: Cảm biến siêu âm →](bai-12-sieu-am.md)
