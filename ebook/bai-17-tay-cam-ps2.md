# Bài 17 – Tay cầm PS2 không dây

> **Mục tiêu:** điều khiển robot bằng tay cầm PS2 dùng sóng RF 2.4 GHz, lái kiểu "arcade", kết hợp servo kẹp.
> **Phần cứng:** bộ tay cầm PS2 không dây + bộ thu cắm cổng **PS2** (DAT IO7, CMD IO2, SEL IO10, CLK IO6), xe 2 bánh, servo kẹp S1.
> **Thư viện:** PS2X_lib.
> **Code:** [`code/17_TayCamPS2`](../code/17_TayCamPS2/17_TayCamPS2.ino)

## 1. Đây là cách điều khiển RF đầu tiên của chúng ta

Tay cầm PS2 không dây gửi trạng thái nút bấm qua sóng **RF 2.4 GHz** tới bộ thu. ESP32 không cần biết chuyện vô tuyến: nó chỉ "hỏi" bộ thu qua giao tiếp SPI rằng nút nào đang được bấm.

```mermaid
flowchart LR
  TC[Tay cầm PS2] -- "RF 2.4 GHz" --> BT[Bộ thu]
  BT -- "SPI: DAT CMD SEL CLK" --> ESP[ESP32-C3]
  ESP -- I2C --> PCA[PCA9685] --> M[Động cơ, servo]
```

## 2. Đọc tay cầm

```cpp
ps2x.config_gamepad(TB_PS2_CLK, TB_PS2_CMD, TB_PS2_SEL, TB_PS2_DAT, false, false); // 0 = OK
ps2x.read_gamepad(false, 0);     // goi moi vong loop
ps2x.Button(PSB_PAD_UP);         // true khi DANG GIU
ps2x.ButtonPressed(PSB_SELECT);  // true DUNG 1 LAN khi vua bam
ps2x.Analog(PSS_LY);             // 0 (day len) .. 128 (giua) .. 255 (keo xuong)
```

| Hằng số | Nút |
|---|---|
| `PSB_PAD_UP / DOWN / LEFT / RIGHT` | Mũi tên |
| `PSB_TRIANGLE / CIRCLE / CROSS / SQUARE` | ▲ ● ✕ ■ |
| `PSB_L1 / L2 / R1 / R2` | Cò vai |
| `PSB_START / SELECT` | |
| `PSS_LX / LY / RX / RY` | Cần analog |

## 3. Lái kiểu arcade (một cần)

Một cần analog cho ra 2 giá trị: **ga** (đẩy lên/xuống) và **rẽ** (trái/phải). Hai giá trị này được trộn thành tốc độ cho 2 bánh:

```
trái  = ga + rẽ
phải  = ga − rẽ
```

| Cần | ga | rẽ | trái | phải | Kết quả |
|---|---|---|---|---|---|
| Đẩy thẳng lên | 100 | 0 | 100 | 100 | Đi thẳng |
| Đẩy sang phải | 0 | 100 | 100 | −100 | Xoay phải tại chỗ |
| Lên + chéo phải | 70 | 30 | 100 | 40 | Rẽ cung phải |

**Vùng chết (deadzone):** cần analog khi thả tay hiếm khi nằm đúng 128 mà thường lệch vài đơn vị, khiến xe bò chậm dù không ai chạm vào. Ta bỏ qua những giá trị lệch nhỏ:

```cpp
int stick(int raw) {
  int v = raw - 128;
  if (abs(v) < DEADZONE) return 0;
  return constrain(map(v, -128, 127, -100, 100), -100, 100);
}
```

## 4. Xử lý sự cố

| Mã lỗi `config_gamepad` | Ý nghĩa |
|---|---|
| 0 | OK |
| 1 | Không tìm thấy tay cầm: kiểm tra dây, bật tay cầm, đợi đèn bộ thu ngừng nháy |
| 2 | Tìm thấy nhưng không nhận lệnh |
| 3 | Không vào được chế độ analog: bấm nút **ANALOG** trên tay cầm |

## 5. Thử thách

1. Lái kiểu **xe tăng**: cần trái điều khiển bánh trái, cần phải điều khiển bánh phải.
2. Dùng cần phải điều khiển servo S2 (nâng hạ tay gắp) **theo tỉ lệ**.
3. Bấm ▲ ba lần liên tiếp trong vòng 1 giây thì robot phát nhạc "chiến thắng".

---

[← Bài 16](bai-16-xe-tranh-vat-can.md) · [Mục lục](../README.md) · Tiếp: [Bài 18 – Wi-Fi →](bai-18-wifi.md)
