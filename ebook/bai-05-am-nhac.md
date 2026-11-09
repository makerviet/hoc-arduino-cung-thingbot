# Bài 5 – Còi và âm nhạc

> **Mục tiêu:** tạo âm thanh bằng tần số, dùng **mảng** để lưu bản nhạc.
> **Code:** [`code/05_AmNhac`](../code/05_AmNhac/05_AmNhac.ino)

## 1. Âm thanh là tần số

Còi trên ThingBot là **còi thụ động**: chỉ phát ra tiếng khi ta cho nó rung bằng sóng vuông. Sóng rung càng nhanh thì âm càng cao.

| Nốt | Đô | Rê | Mi | Fa | Sol | La | Si | Đô cao |
|---|---|---|---|---|---|---|---|---|
| Tên quốc tế | C4 | D4 | E4 | F4 | G4 | A4 | B4 | C5 |
| Tần số (Hz) | 262 | 294 | 330 | 349 | 392 | 440 | 494 | 523 |

Còi nối vào kênh 14 của PCA9685. Để phát được nốt La (440 Hz), ta đổi tần số của **cả chip** PCA9685 thành 440 Hz rồi đặt độ rộng xung 50%:

```cpp
void playNote(int hz, float beats) {
  int ms = beats * 60000.0 / bpm;   // doi phach -> mili giay
  if (hz > 0) {
    pwm.setPWMFreq(hz);
    pwm.setPWM(BUZZER, 0, 2048);    // 2048/4096 = 50%
  }
  delay(ms - 50);
  pwm.setPWM(BUZZER, 0, 0);
  pwm.setPWMFreq(50);               // tra lai 50 Hz
  delay(50);                        // ngat giua 2 not
}
```

> **Lưu ý:** PCA9685 chỉ có **một** tần số chung cho cả 16 kênh. Trong lúc phát nhạc, servo (cần đúng 50 Hz) sẽ quay lung tung. Vì vậy đừng phát nhạc trong lúc servo đang làm việc. Dải tần phát được khoảng **24 – 1526 Hz**.

## 2. Mảng (array)

Một bài hát gồm nhiều nốt. Thay vì tạo 25 biến riêng, ta dùng **mảng**, tức một dãy giá trị có đánh số thứ tự:

```cpp
const int melody[] = {C4, C4, D4, C4, F4, E4, ...};
const float beats[] = {0.75, 0.25, 1, 1, 1, 2, ...};
const int N = sizeof(melody) / sizeof(melody[0]);  // so phan tu

for (int i = 0; i < N; i++) {
  playNote(melody[i], beats[i]);
}
```

- `melody[0]` là phần tử **đầu tiên**. Chỉ số bắt đầu từ **0**.
- `sizeof(melody)` là tổng số byte của mảng, chia cho số byte của một phần tử thì ra số phần tử.
- `const` báo rằng mảng không bao giờ thay đổi.

## 3. Nhịp (BPM)

`bpm = 120` nghĩa là 120 phách mỗi phút, tức mỗi phách dài 500 ms. Nốt 2 phách dài 1 giây, nốt 0,25 phách dài 125 ms.

## 4. Thử thách

1. Chép bài "Kìa con bướm vàng" (Đô Rê Mi Đô | Đô Rê Mi Đô | Mi Fa Sol | Mi Fa Sol…).
2. Thêm biến đổi nhịp: mỗi lần bấm SW thì bài hát nhanh hơn 20 BPM.
3. Làm tiếng còi xe cứu thương: luân phiên 700 Hz và 1000 Hz.

---

[← Bài 4](bai-04-nut-bam.md) · [Mục lục](../README.md) · Tiếp: [Bài 6 – Đa nhiệm với millis() →](bai-06-da-nhiem-millis.md)
