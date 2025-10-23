# 회로도 및 설계도

## 하드웨어 연결도

### Arduino Uno 핀 배치

```
Digital Pins:
- Pin 3: 현재 버스 정류장 확인 버튼 (NOW_BUS_STOP)
- Pin 4: 하차 버튼 (Stopover)
- Pin 5: 하차 LED (Stopover_LED)
- Pin 6: 키패드 LED (Keypad_LED)
- Pin 7: 볼륨 업 버튼 (SOUND_UP_BUTTON)
- Pin 8: 볼륨 다운 버튼 (SOUND_DOWN_BUTTON)
- Pin 9: 블루투스 RX (RXD)
- Pin 10: 블루투스 TX (TXD)
- Pin 11: 부저 (buzzer)
- Pin 22-33: 키패드 매트릭스
```

### 키패드 매트릭스 연결

```
키패드 레이아웃:
[1] [2] [3]  → Pin 22, 23, 24
[4] [5] [6]  → Pin 25, 26, 27
[7] [8] [9]  → Pin 28, 29, 30
[*] [0] [#]  → Pin 31, 32, 33

각 키에 해당하는 코드값:
201 202 203
204 205 206
207 208 209
211 210 212
```

### 블루투스 모듈 (HC-06) 연결

```
HC-06    Arduino
VCC   →  5V
GND   →  GND
TXD   →  Pin 9 (RXD)
RXD   →  Pin 10 (TXD)
```

### 부저 및 LED 연결

```
부저:
- 양극(+) → Pin 11
- 음극(-) → GND

하차 LED:
- 애노드(+) → Pin 5
- 캐소드(-) → GND (저항 220Ω 경유)

키패드 LED:
- 애노드(+) → Pin 6
- 캐소드(-) → GND (저항 220Ω 경유)
```

### 버튼 연결

```
모든 버튼은 풀업 저항 사용 (INPUT_PULLUP)
- 한쪽 → 해당 핀
- 다른쪽 → GND
```

## 주파수 매핑 (부저)

각 키패드 버튼에 해당하는 음계:

```cpp
int buzzer_freq[4][3] = {
  { 523, 554, 587 },  // C5, C#5, D5
  { 622, 659, 698 },  // D#5, E5, F5
  { 739, 783, 830 },  // F#5, G5, G#5
  { 880, 932, 987 }   // A5, A#5, B5
};
```

## 전원 공급

- Arduino Uno: USB 전원 또는 DC 7-12V
- 전체 시스템 소비전력: 약 200mA @ 5V
