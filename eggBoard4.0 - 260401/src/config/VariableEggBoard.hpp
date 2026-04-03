/* VariableEggBoard.h */
#ifndef VARIABLE_EGG_BOARD_H
#define VARIABLE_EGG_BOARD_H

#include <Arduino.h>
#include "Define.hpp"
#include <DueFlashStorage.h>
#include <SimpleTimer.h>
#include <Adafruit_PWMServoDriver.h>

extern boolean   calibration;
extern boolean   calFirstTime;
extern boolean   exitOrderASC;      // BAD EGG EXIT 순번(true: 0 번부터 출력, false: 5번부터 출력)
extern int       pulseCount_0;
extern int       pulseCount_1;
extern int       pulseCount_2;
extern int       pulseCount_3;
extern int       pulseHOMECounter;
 
struct TimingParams {
    unsigned long period[3];   // period[0] ~ period[2]
    unsigned long middle[2];   // middle[0] ~ middle[1]
    unsigned long calibrationTime;
};
extern TimingParams timingParams;
 
// MACHINE SET
#define   LOADCELL_TOTAL    50              // 전체 Loadcel 개수 (MAX 설정으로 이 안에서 해결)
// MACHINE SET
struct SystemFlags {
  boolean   eggSTART         ;
  boolean   timerSTART       ;
  boolean   manualSTART      ;
  boolean   triggerSTART     ;
  boolean   triggerSTARTpin  ;
  boolean   eggExitSTART     ;
  boolean   eggBadExitSTART  ;
  boolean   ledSTART         ;
  boolean   extPackeRcv;
  boolean   badPackeRcv;
  boolean   extBadPacketRcv;         // Ext and Bad packet total received status (2022.01.29)
};
extern SystemFlags systemFlags; 

// SystemFlags 매크로 치환
#define eggSTART        systemFlags.eggSTART
#define timerSTART      systemFlags.timerSTART
#define manualSTART     systemFlags.manualSTART
#define triggerSTART    systemFlags.triggerSTART
#define triggerSTARTpin systemFlags.triggerSTARTpin
#define eggExitSTART    systemFlags.eggExitSTART
#define eggBadExitSTART systemFlags.eggBadExitSTART
#define ledSTART        systemFlags.ledSTART
#define extPackeRcv     systemFlags.extPackeRcv
#define badPackeRcv     systemFlags.badPackeRcv
#define extBadPacketRcv systemFlags.extBadPacketRcv

extern int runMode;
extern int ledOnPosition;
extern int ledBrightness;
extern unsigned long ledDuration;
extern unsigned long triggerDelay;
extern unsigned long triggerDuration;
extern int optionBits;
extern int proximityStep;
extern int loadCellStep;
extern int outputPosition;
extern int outputDuration;
extern int badOutputStep;
extern int badOutputPosition;
extern int badOutputDuration;
extern int homePlus;
extern bool nowHOMEposition;

extern   int           ledPinArray[3][ledCount];     // 0: no, 1:bank address
extern   byte          ledEnvArray[2][60];           // 0:brightness, 1:duration
extern   unsigned long ledEnvArrayInt[2][ledCount];        // 0:brightness, 1:duration [int value changed array for handling process]
extern   unsigned long ledPreTimeArray[ledCount];          // LED OFF management Array for check shutdown time check [2022.01.25]
extern int nowLedOffCount;
extern boolean ledEachStart;

extern byte ledLight1;
extern byte ledLight2;
extern byte ledLight3;
extern byte ledLight4;
extern byte ledLight5;

 // Manual mode Variables

extern byte eggExistBit_manual;
extern byte crackStatusBit_manual;
extern byte badStatusBit_manual;
extern int manualTimerPeriod;
extern boolean manualModeStart;
extern boolean timerModeStart;

struct eggBoardEnv {
    int runMode;
    int ledOnPosition;
    int ledBrightness;
    int ledDuration;
    int triggerDelay;
    int triggerDuration;
    int optionBits;
    int proximityStep;
    int loadCellStep;
    int outputPosition;
    int outputDuration;
    int badOutputStep;
    int badOutputPosition;
    int badOutputDuration;
    int homePlus;
};
extern eggBoardEnv eggBoard;
extern int eeAddress;

 //Timer values set
 #define nearSensorInterval          2000     // milisec
 #define nearSensorPacketInterval    4000     // milisec

extern int sensor1;
extern int sensor2;
extern int sensor3;
extern int sensor4;
extern int sensor5;
extern int sensor6;
extern int sensor7;
extern int sensor8;

extern const int NEAR_SENSOR_PINS[8];
extern boolean sensor_status[8];
extern unsigned long eggNoPreviousTime;

extern volatile int a_counter;
extern volatile int b_counter;
extern volatile int oneStepCounter;
extern int previous_counter;
extern int currentStateClock;
extern int StateData;
extern int isReset;
extern int lastStateClock;
extern int homePosition;

extern boolean FirstRun;
extern boolean oneStepMove;
extern boolean innerLoop;
extern unsigned long previousTime;
extern unsigned long nearPreviousTime;
extern unsigned long ledPreTime;
extern unsigned long triggerDelayPreTime;
extern unsigned long triggerOffPreTime;
extern unsigned long triggerDurationPreTime;
extern unsigned long eggExitDelayPreTime;
extern unsigned long eggExitDurationPreTime;
extern unsigned long eggBadExitDelayPreTime;
extern unsigned long eggBadExitDurationPreTime;
extern unsigned long eggStartPreTime;
extern int eggStartCount;
extern int programCount;
extern int serialEncoderMax;
#define  serialEndocerStep   20            // serialEncoderMax 값에서 최대 Encoder max value
extern int nowEncoderCount;

extern unsigned long previousTimerTime;
extern unsigned long ledPreTimerTime;
extern unsigned long ledOnDelayPreTime;
extern int ledTimerCount;
extern int triggerTimerCount;

extern int programSequence;
extern byte programSequenceHex[2];

extern byte exitEggValue;
extern byte exitBadEggValue;

extern char toCheck[200];
extern int bufSize;

extern unsigned long currentTime;
extern unsigned long loopCount;
extern unsigned long triggerCount;
extern unsigned long triggerOffCount;
extern unsigned long triggerLoopCount;
extern unsigned long eggExitDelayCount;
extern unsigned long eggExitCount;
extern unsigned long badExitDelayCount;
extern unsigned long badExitCount;
extern unsigned long eggExitLoopCount;
extern unsigned long badExitLoopCount;
extern unsigned long ledOnCount;
extern unsigned long ledOnDelayCount;
extern unsigned long ledOnOffLoopCNT;

extern byte eggBoardLoadCell[2][LOADCELL_TOTAL];
extern int appliedLoadCellCount;

extern unsigned long lastMsgTime;

extern char Test_Flag ;

  // Interrupt related variables
extern volatile bool g_homeEvent;
extern volatile bool g_stepBoundaryEvent;

extern Adafruit_PWMServoDriver ledBank0;
extern Adafruit_PWMServoDriver ledBank1;

extern const uint8_t CRACK_PINS[8];
extern const uint8_t BAD_PINS[8];
extern const uint8_t TRIGGER_PINS[4];
#endif