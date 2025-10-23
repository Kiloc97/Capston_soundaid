# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

## [1.0.0] - 2025-01-24

### Added

- 초기 Sound Aid 시스템 구현
- 블루투스 HC-06 모듈을 통한 스마트폰 연동
- 4x3 키패드를 이용한 정류장 번호 입력 시스템
- 볼륨 조절 기능 (0-10 단계)
- 현재 정류장 확인 기능
- 하차 예약 및 알림 시스템
- LED 및 부저를 통한 시각/청각 피드백
- 자동 정류장 업데이트 시뮬레이션

### Features

- **볼륨 제어**: 업/다운 버튼으로 음성 안내 볼륨 조절
- **정류장 예약**: 키패드로 목적지 입력 후 '#'로 예약
- **예약 취소**: '\*' 키로 예약 취소
- **도착 알림**: 목적지 1정거장 전 자동 알림
- **하차 벨**: 하차 의사를 기사에게 전달하는 버튼
- **음성 피드백**: 각 키 입력 시 고유한 음계 출력

### Technical Details

- Arduino Uno 기반 설계
- SoftwareSerial 라이브러리를 이용한 블루투스 통신
- 디바운스 처리로 버튼 입력 안정성 확보
- 키패드 매트릭스 스캔 방식 구현
- 모듈화된 함수 구조로 유지보수성 향상

### Hardware Components

- Arduino Uno
- HC-06 블루투스 모듈
- 4x3 매트릭스 키패드
- 푸시 버튼 x 4개
- LED x 2개
- 부저 x 1개
- 저항, 점퍼 와이어 등 기타 부품
