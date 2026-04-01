#include <Arduino.h>
#include "EggExit/EggExit.h"
#include "config/Define.hpp"
#include "config/VariableEggBoard.hpp"

// 계란 외관 핀 ON
void EggExit::crackExitOn(byte data) {
for (int i = 0; i < 8; i++) {
    if (bitRead(data, i)) digitalWrite(CRACK_PINS[i], HIGH);
}
}

// 계란 외관 핀 OFF
void EggExit::crackExitOff(byte data) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(CRACK_PINS[i], LOW);
    }
}

// 불량 계란 출력 ON
void EggExit::badExitOn(byte data) {
    for (int i = 0; i < 8; i++) {
        if (bitRead(data, i)) digitalWrite(BAD_PINS[i], HIGH);
    }
}

// 불량 계란 출력 OFF
void EggExit::badExitOff(byte data) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(BAD_PINS[i], LOW);
    }
}

// 불량 계란 1개 출력 ON
void EggExit::badExitOnOne(byte data, int count) {
    if (bitRead(data, count)) digitalWrite(BAD_EXIT1, HIGH);
}

// 불량 계란 1개 출력 OFF
void EggExit::badExitOffOne() {
    digitalWrite(BAD_EXIT1, LOW);
}

