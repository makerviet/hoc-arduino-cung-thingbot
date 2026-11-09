# Bài 7 – Thư viện ThingBot

> **Mục tiêu:** hiểu thư viện được tạo ra thế nào, và chuyển sang dùng thư viện ThingBot cho code gọn hơn.
> **Code:** [`code/07_ThuVienThingBot`](../code/07_ThuVienThingBot/07_ThuVienThingBot.ino) · Thư viện: [`libraries/ThingBot`](../libraries/ThingBot)

## 1. Vì sao cần thư viện riêng?

Ở Phần 1, chương trình nào cũng bắt đầu bằng khoảng 10 dòng giống hệt nhau (`#include`, `#define`, `pwm.begin()`…). Các phần sau còn cần thêm động cơ, servo và siêu âm, nên code sẽ dài thêm nữa. Thư viện **ThingBot** gói tất cả lại:

| Phần 1 (tự viết) | Từ Bài 7 (thư viện) |
|---|---|
| `pwm.begin(); pwm.setOscillatorFrequency(...); pwm.setPWMFreq(50); pinMode(SW, INPUT);` | `ThingBot.begin();` |
| `pwm.setPin(15, map(p, 0, 100, 0, 4095));` | `ThingBot.led(1, p);` |
| Code phát hiện cạnh bấm + chống dội (Bài 4) | `ThingBot.buttonClicked()` |
| Hàm `playNote` (Bài 5) | `ThingBot.playNote(NOTE_C5, 1);` |

> Thư viện không có "phép màu" gì. Mở [`ThingBot.cpp`](../libraries/ThingBot/src/ThingBot.cpp) ra, bạn sẽ thấy đúng những dòng code đã học ở Phần 1.

## 2. Cấu trúc một thư viện Arduino

```
libraries/ThingBot/
├── library.properties     ← ten, phien ban, thu vien phu thuoc
├── src/
│   ├── ThingBot.h         ← "muc luc": khai bao cac ham
│   └── ThingBot.cpp       ← "noi dung": code that cua cac ham
└── examples/ThingBotTest/ ← vi du (hien trong File > Examples)
```

- **File `.h` (header)** liệt kê thư viện có những hàm gì, nhận tham số gì.
- **File `.cpp`** chứa phần code thực hiện các hàm đó.
- `class ThingBotClass { ... };` gom dữ liệu và hàm thành một **lớp**. `ThingBot` là **đối tượng** duy nhất thuộc lớp đó, được tạo sẵn trong thư viện.

## 3. Code

```cpp
#include <ThingBot.h>

int level = 0;

void setup() {
  Serial.begin(115200);
  if (!ThingBot.begin()) Serial.println("Khong tim thay PCA9685!");
  ThingBot.beep();
}

void loop() {
  if (ThingBot.buttonClicked()) {
    level = (level + 1) % 5;            // 0,1,2,3,4,0,...
    ThingBot.led(1, level * 25);
    ThingBot.led(2, 100 - level * 25);
    ThingBot.playNote(NOTE_C5 + level * 100, 0.25);
  }
}
```

- `ThingBot.begin()` trả về `false` nếu không tìm thấy chip PCA9685, giúp phát hiện lỗi phần cứng sớm.
- `%` là phép **chia lấy dư**: `5 % 5 = 0`, nên `level` chạy vòng 0 → 4 rồi quay về 0.

## 4. Kiểm tra phần cứng

Thư viện có sẵn ví dụ **File → Examples → ThingBot → ThingBotTest**. Ví dụ này kiểm tra lần lượt LED, còi, 4 động cơ, 5 servo và siêu âm. Hãy chạy nó mỗi khi lắp robot mới.

Danh sách đầy đủ các hàm: [Phụ lục A – Tham khảo thư viện](phu-luc-a-thu-vien-thingbot.md).

## 5. Thử thách

1. Viết lại Bài 6 (đa nhiệm) bằng thư viện ThingBot. Code ngắn đi bao nhiêu dòng?
2. Thêm vào thư viện hàm `void ThingBotClass::blink(uint8_t which, int times)`. Nhớ khai báo hàm trong `.h` và viết code trong `.cpp`.

---

[← Bài 6](bai-06-da-nhiem-millis.md) · [Mục lục](../README.md) · Tiếp: [Bài 8 – Động cơ DC →](bai-08-dong-co.md)
