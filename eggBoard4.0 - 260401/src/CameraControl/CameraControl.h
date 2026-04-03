#ifndef TRIGGER_CONTROL_H
#define TRIGGER_CONTROL_H

#include <Arduino.h>

class TriggerController {
    public: 
        static void triggerOn();
        static void triggerOff();
        static void numberCheck();
        static void setCameraState(int index, bool on);
        // 외관 카메라
        void exteriorCamera();
        // 투광 카메라
        void floodlightCamera();
        // 카메라 통합 박스에 외관 + 투광
        void integration();
        // 카메라 직선 상의 2개의 박스 (외관 , 투광)
        void LinearIntegration();
};

#endif