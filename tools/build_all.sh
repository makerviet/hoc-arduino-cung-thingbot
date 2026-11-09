#!/usr/bin/env bash
# Bien dich kiem tra toan bo sketch trong code/ va vi du thu vien.
# Yeu cau: arduino-cli, core esp32:esp32 (3.x), va cac thu vien:
#   Adafruit PWM Servo Driver Library, Adafruit SSD1306, Adafruit GFX Library,
#   DHT sensor library, Adafruit MPU6050, Adafruit Unified Sensor, PS2X_lib
# Them thu muc thu vien bo sung bang bien EXTRA_LIBS="--library duong/dan ..."
set -u
cd "$(dirname "$0")/.."

FQBN="esp32:esp32:esp32c3:CDCOnBoot=cdc"
LIBS="--library libraries/ThingBot ${EXTRA_LIBS:-}"
pass=0
fail=0

for dir in code/*/ libraries/ThingBot/examples/*/; do
  if out=$(arduino-cli compile -b "$FQBN" $LIBS "$dir" 2>&1); then
    echo "OK    $dir"
    pass=$((pass + 1))
  else
    echo "FAIL  $dir"
    echo "$out" | grep -E "error|Error" | head -5
    fail=$((fail + 1))
  fi
done

echo "----"
echo "OK: $pass   FAIL: $fail"
[ "$fail" -eq 0 ]
