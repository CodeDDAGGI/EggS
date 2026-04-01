#include <Arduino.h> 
#include "config/Define.hpp"
#include "config/VariableEggBoard.hpp"
#include "CameraControl/CameraControl.h"


void TriggerController::triggerOn() {
    for (int i = 0; i < 4; i++) digitalWrite(TRIGGER_PINS[i], HIGH);
}

void TriggerController::triggerOff() {
    for (int i = 0; i < 4; i++) digitalWrite(TRIGGER_PINS[i], LOW);
}

void TriggerController::numberCheck(){
    
}

void TriggerController::setCameraState(int index, bool on) {
    if (index < 0 || index >= 4) return;
    digitalWrite(TRIGGER_PINS[index], on ? LOW : HIGH);
}


// 외관 카메라
void TriggerController::exteriorCamera(){

}

// 투광 카메라
void TriggerController::floodlightCamera(){

}

//
void TriggerController::integration() {
    // 카메라 통합 박스에 외관 + 투광
    digitalWrite(TRIGGER_PINS[0], LOW); // 외관 카메라 ON
    digitalWrite(TRIGGER_PINS[1], LOW); // 투광 카메라 ON
}

void TriggerController::LinearIntegration() {
    // 카메라 직선 상의 2개의 박스 (외관 , 투광)
    digitalWrite(TRIGGER_PINS[0], LOW); // 외관 카메라 ON
    digitalWrite(TRIGGER_PINS[1], LOW); // 투광 카메라 ON
}
