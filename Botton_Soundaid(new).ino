#include <SoftwareSerial.h>

#define TXD 10
#define RXD 9

// 버튼 및 가변저항 핀 설정
#define SOUND_UP_BUTTON 7     // 볼륨 업 버튼
#define SOUND_DOWN_BUTTON 
8   // 볼륨 다운 버튼
#define NOW_BUS_STOP 3        // 현재 버스 정류장 확인 버튼
#define Stopover 4            // 하차 버튼
#define Stopover_LED 5        // 하차 LED
#define Keypad_LED 6          // 키패트 LED
#define buzzer 11             // 키패트 부저
//#define Reservation A0        // 가변저항 (다이얼)
//#define Reservation_button 6  // 예약 버튼

#define ROWS 4
#define COLS 3

// 블루투스 모듈 설정
SoftwareSerial hc06(TXD, RXD);

int volume = 5;             // 현재 볼륨
int busstop = 0;            // 현재 버스 정류장
bool stopoverFlag = false;  // 하차 벨 상태
int reservedStop = -1;      // 예약된 정류장 (-1은 예약 없음)
//int selectedStop = 0;
int distanceToStop = 0;

int SWPin[ROWS][COLS] = { { 22, 23, 24 },
                          { 25, 26, -10 },
                          { -10, -10, -10 },
                          { 31, 32, 33 } };

int keys[ROWS][COLS] = {
  { 201, 202, 203 },
  { 204, 205, 206 },
  { 207, 208, 209 },
  { 211, 210, 212 }
};

int buzzer_freq[ROWS][COLS] = {
  { 523, 554, 587 },
  { 622, 659, 698 },
  { 739, 783, 830 },
  { 880, 932, 987 }
};
//#: 예약
//*: 취소

// 초기화
void setup() {
  Serial.begin(9600);
  hc06.begin(9600);

  pinMode(SOUND_UP_BUTTON, INPUT_PULLUP);
  pinMode(SOUND_DOWN_BUTTON, INPUT_PULLUP);
  pinMode(NOW_BUS_STOP, INPUT_PULLUP);
  pinMode(Stopover, INPUT_PULLUP);
  pinMode(Stopover_LED, OUTPUT);
  pinMode(Keypad_LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //pinMode(Reservation_button, INPUT_PULLUP);
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      pinMode(SWPin[r][c], INPUT_PULLUP);
    }
  }
}

void loop() {
  handleVolumeControl();   // 볼륨 조절
  handleBusStopUpdate();   // 정류장 업데이트
  handleStopoverButton();  // 하차 버튼
  handleCurrentBusStop();  // 현재 정류장 확인 버튼
  handleReservation();     // 예약 기능
  delay(100);
}

// 🧩 볼륨 조절 함수
void handleVolumeControl() {
  if (digitalRead(SOUND_UP_BUTTON) == HIGH) {
    volume = constrain(volume + 1, 0, 10);
    hc06.println(volume);
    delay(200);  // debounce
  } else if (digitalRead(SOUND_DOWN_BUTTON) == HIGH) {
    volume = constrain(volume - 1, 0, 10);
    hc06.println(volume);
    delay(200);  // debounce
  }
}

// 🧩 버스 정류장 업데이트 함수
void handleBusStopUpdate() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {  // 5초마다 다음 정류장으로 이동
    busstop = ((busstop + 1) % 5)+300;
    stopoverFlag = false;  // 하차 벨 리셋
    digitalWrite(Stopover_LED, LOW);
    lastUpdate = millis();
  }
}

// 🧩 하차 버튼 처리 함수
void handleStopoverButton() {
  if (digitalRead(Stopover) == HIGH) {
    stopoverFlag = true;
    digitalWrite(Stopover_LED, HIGH);
    tone(buzzer, 250);
    delay(500);  // debounce
    noTone(buzzer);
  }
}

// 🧩 현재 정류장 확인 버튼
void handleCurrentBusStop() {
  static bool lastState = LOW;
  if (digitalRead(NOW_BUS_STOP) == HIGH && lastState == LOW) {
    hc06.println("400");
  }
  lastState = digitalRead(NOW_BUS_STOP);
}

// 🧩 키패드 입력 처리 함수
int SWKeyPad_NUM() {
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      if (digitalRead(SWPin[r][c]) == HIGH) {
        digitalWrite(Keypad_LED, HIGH);
        tone(buzzer, buzzer_freq[r][c]);
        Serial.println("버튼" + String(keys[r][c]));
        delay(200);
        return keys[r][c];
      }
    }
  }
  digitalWrite(Keypad_LED, LOW);
  noTone(buzzer);
  return -1;
}
/*
22 23 24
25 26 27
28 29 30
31 32 33
*/
// 🧩 예약 기능 처리 함수
void handleReservation() {
  /*
  // 아날로그 입력 값에 따른 선택 정류장 갱신
  int newSelectedStop = map(analogRead(Reservation), 0, 1023, 1, 9);
  if (selectedStop != newSelectedStop) {
    selectedStop = newSelectedStop;
    hc06.println(selectedStop);  // selectedStop 값이 변경된 경우에만 출력
    delay(200);
  }
  */
  int newSelectedStop = SWKeyPad_NUM();
  if (newSelectedStop != -1) {
    if (newSelectedStop == '*') {
      reservedStop = -1;
      hc06.println(reservedStop);
    } else if (newSelectedStop == '#') {
      reservedStop = (busstop + newSelectedStop) % 5;
      //선택된 버스 정류장 출력
      hc06.println(reservedStop);
    }else{
       hc06.println(newSelectedStop);
    }
    delay(200);
  }
/*
  // 예약 버튼이 눌린 경우
  if (digitalRead(digitalRead(SWPin[3][2]) == HIGH) {
    //버스 정류장 선택
    reservedStop = (busstop + selectedStop) % 10;
    //선택된 버스 정류장 출력
    hc06.println(reservedStop);
    delay(200);  // debounce
  }
  if(digitalRead(digitalRead(SWPin[3][0]{
    reservedStop = -1;
    delay(200);  // debounce
  }
*/

  // 🧩 예약된 정류장과 현재 정류장 간의 거리 계산 후 전송
   if (reservedStop >= 0) {
    int newDistanceToStop = (reservedStop - busstop + 5) % 5;
    if (distanceToStop != newDistanceToStop) {
      distanceToStop = newDistanceToStop;
      hc06.println(distanceToStop);  // distanceToStop 값이 변경된 경우에만 출력
      if (distanceToStop == 1) {
        tone(buzzer, 250);
        digitalWrite(Stopover_LED, HIGH);
        delay(200);
        noTone(buzzer);
      }
      if (distanceToStop == 0) {
        reservedStop = -1;  // 도달한 경우 예약 취소
      }
    }
  }
}

