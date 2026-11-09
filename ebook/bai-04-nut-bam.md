# Bài 4 – Nút bấm SW

> **Mục tiêu:** đọc tín hiệu số bằng `digitalRead()`, dùng `if/else`, phát hiện cạnh bấm, chống dội phím.
> **Code:** [`code/04_NutBam`](../code/04_NutBam/04_NutBam.ino)

## 1. Đọc nút bấm

Khác với LED, nút **SW** nối thẳng vào chân **IO3** của ESP32:

```cpp
#define SW 3
pinMode(SW, INPUT);                     // trong setup()
bool pressed = (digitalRead(SW) == LOW);  // bam = LOW
```

> Nút trên ThingBot là loại **tích cực mức thấp**: khi bấm, chân bị nối xuống đất (`LOW`). Khi thả, điện trở kéo lên giữ chân ở mức `HIGH`.

## 2. `if / else`

```cpp
if (pressed) {
  pwm.setPin(LED_2, 4095);   // dang giu -> sang
} else {
  pwm.setPin(LED_2, 0);      // tha ra -> tat
}
```

## 3. Đếm số lần bấm: phát hiện cạnh

`loop()` chạy hàng nghìn lần mỗi giây. Nếu cứ mỗi lần thấy `pressed == true` là đếm thêm 1, thì một cú bấm sẽ bị đếm hàng nghìn lần. Ta chỉ đếm tại **thời điểm chuyển** từ *thả* sang *bấm*, gọi là **cạnh bấm**:

```cpp
if (pressed && !lastPressed) {   // lan truoc chua bam, lan nay bam
  led1On = !led1On;              // dao trang thai
  clicks++;
  delay(30);                     // chong doi phim
}
lastPressed = pressed;
```

```
Tín hiệu SW:   ‾‾‾‾‾‾\_/‾\_____________/‾‾‾‾‾‾
                     ↑ dội phím (vài ms)
Cạnh bấm:             ↑ (chỉ đếm 1 lần)
```

**Dội phím (bounce):** lá kim loại trong nút nảy lên nảy xuống vài mili giây lúc bấm, nên tín hiệu bị nhiễu. Nghỉ 30 ms sau cạnh bấm giúp bỏ qua đoạn nhiễu này.

| Toán tử | Ý nghĩa |
|---|---|
| `&&` | VÀ (cả hai đều đúng) |
| `\|\|` | HOẶC |
| `!` | PHỦ ĐỊNH (`!true` là `false`) |
| `a ? b : c` | Nếu `a` đúng thì lấy `b`, ngược lại lấy `c` |

## 4. Thử thách

1. Bấm 1 lần: LED_1 sáng. Bấm lần 2: LED_2 sáng. Bấm lần 3: tắt cả hai. Rồi lặp lại.
2. **Giữ** nút hơn 2 giây thì cả 2 đèn nháy (gợi ý: dùng `millis()`, sẽ học kỹ ở Bài 6).
3. Trò chơi phản xạ: đèn sáng sau một khoảng ngẫu nhiên `random(1000, 5000)` ms, đo xem người chơi bấm nhanh tới đâu.

---

[← Bài 3](bai-03-sang-dan.md) · [Mục lục](../README.md) · Tiếp: [Bài 5 – Âm nhạc →](bai-05-am-nhac.md)
