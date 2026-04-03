#ifndef _VARIABLE_EGG_BOARD
#define _VARIABLE_EGG_BOARD

#include <Arduino.h>
#include "config/VariableEggBoard.hpp"
#include "config/Define.hpp"
#include <Adafruit_PWMServoDriver.h>


boolean   calibration =     false;
boolean   calFirstTime =    true;
boolean   exitOrderASC =    true;      // BAD EGG EXIT 순번(true: 0 번부터 출력, false: 5번부터 출력)
int       pulseCount_0 =    0;
int       pulseCount_1 =    0;
int       pulseCount_2 =    0;
int       pulseCount_3 =    0;
int       pulseHOMECounter = 0;

unsigned long       timePeriod_1 =    0;
unsigned long       timePeriod_2 =    0;
unsigned long       timePeriod_3 =    0;
unsigned long       timeMiddle_1 =    0;
unsigned long       timeMiddle_2 =    0;
unsigned long       calibration_time = 0;


// MACHINE SET
SystemFlags systemFlags = {
    false, // eggStart
    false, // timerStart
    false, // manualStart
    false, // triggerStart
    false, // triggerStartPin
    false, // eggExitStart
    false, // eggBadExitStart
    false, // ledStart
    false, // extPacketReceived
    false, // badPacketReceived
    false  // extBadPacketReceived
};

  // SETTING VALUES
  int     runMode =           0;
  int     ledOnPosition =     0;
  int     ledBrightness =     0;
  unsigned long     ledDuration =       0;
  unsigned long     triggerDelay =      0;
  unsigned long     triggerDuration =   0;
  int     optionBits =        0;
  int     proximityStep =     0;
  int     loadCellStep =      0;
  int     outputPosition =    0;
  int     outputDuration    = 0;
  int     badOutputStep     = 0;
  int     badOutputPosition = 0;
  int     badOutputDuration = 0;
  int     homePlus =          0;
  bool    nowHOMEposition =   false;

  // LED ARRAY
  
  int           ledPinArray[3][ledCount];     // 0: no, 1:bank address
  byte          ledEnvArray[2][60];           // 0:brightness, 1:duration
  unsigned long ledEnvArrayInt[2][ledCount];        // 0:brightness, 1:duration [int value changed array for handling process]
  unsigned long ledPreTimeArray[ledCount];          // LED OFF management Array for check shutdown time check [2022.01.25]
  int           nowLedOffCount =  0;          // LED OFF check count (in order to check how much led off now !! max : 30)
  boolean       ledEachStart =    false;

  byte    ledLight1 =  0;
  byte    ledLight2 =  0;
  byte    ledLight3 =  0;
  byte    ledLight4 =  0;
  byte    ledLight5 =  0;

  // Manual mode Variables
  byte        eggExistBit_manual =    0b00000000;
  byte        crackStatusBit_manual = 0b00000000;
  byte        badStatusBit_manual =   0b00000000;
  int         manualTimerPeriod =     2000;                    // Timer mode duration, default = 2000 
  boolean     manualModeStart =       false;
  boolean     timerModeStart =        false;

  int     eeAddress = 0;

  // NEAR SENSOR READING VALUE
  int     sensor1 = 0;
  int     sensor2 = 0;
  int     sensor3 = 0;
  int     sensor4 = 0;
  int     sensor5 = 0;
  int     sensor6 = 0;
  int     sensor7 = 0;
  int     sensor8 = 0;

  const int NEAR_SENSOR_PINS[8] = {
    NEAR_SENSOR1, NEAR_SENSOR2, NEAR_SENSOR3, NEAR_SENSOR4,
    NEAR_SENSOR5, NEAR_SENSOR6, NEAR_SENSOR7, NEAR_SENSOR8
  };

  boolean           sensor_status[8] =  {false,false,false,false,false,false,false,false};
  unsigned long     eggNoPreviousTime = 0;


  //Rotary Encoder related variables
  volatile int      a_counter =         0;          //Use this variable to store "steps"
  volatile int      b_counter =         0;          //Use this variable to store "steps"
  volatile int      oneStepCounter =    0;          // One_step counter it's reset every 150.
  int               previous_counter =  0;          //Use this variable to store previous "steps" value
  int               currentStateClock = 0;              //Store the status of the clock pin (HIGH or LOW)
  int               StateData = 0;                      //Store the status of the data pin (HIGH or LOW)
  int               isReset = 0;                        //Reset whole the locations (With egges found area !!)
  int               lastStateClock = 0;                 //Store the PREVIOUS status of the clock pin (HIGH or LOW)
  int               homePosition = 0;
  
  //Program Control
  boolean                 FirstRun = true;
  boolean                 oneStepMove = false;              // 1/4 encoder position. this time should work every parts
  boolean                 innerLoop = false;                // timer 맞추기 위한 변수. 최종단에 Array shift 하기 위해 제어 함.
  unsigned long           previousTime = 0;                 // 전체 프로그램 Flow 처리를 위한 TIMER 변수
  unsigned long           nearPreviousTime = 0;             // 근접센서 Flow 처리를 위한 TIMER 변수
  unsigned long           ledPreTime = 0;                   // LED ON Duration 적용을 위한 시간변수
  unsigned long           triggerDelayPreTime = 0;          // TRIGGER DELAY 적용을 위한 시간변수
  unsigned long           triggerOffPreTime = 0;            // TRIGGER OFF를 위한 시간변수
  unsigned long           triggerDurationPreTime = 0;       // TRIGGER DURATION 적용을 위한 시간변수
  unsigned long           eggExitDelayPreTime = 0;          // EGG EXIT DELAY 적용을 위한 시간변수
  unsigned long           eggExitDurationPreTime = 0;       // EGG EXIT DURATION 적용을 위한 시간변수
  unsigned long           eggBadExitDelayPreTime = 0;       //   BAD EGG EXIT DELAY 적용을 위한 시간변수
  unsigned long           eggBadExitDurationPreTime = 0;    // BAD EGG EXIT DURATION 적용을 위한 시간변수
  unsigned long           eggStartPreTime = 0;              // ** 근접센서 접근시 Delay time을 처리하기 위한 시간변수 (최초 0, 6개중 하나라도 ON 이면 무조건 시작, 센서별 시간차를 고려해야 함)
  int                     eggStartCount = 0;
  int                     programCount = 0;                 // 전체 프로그램 제어를 위한 count
  int                     serialEncoderMax = 0;             // Serial read시에 Encoder counter max value setting (2022.01.15), 본 세팅값을 넘겨서 패킷 수신이 안되면 무조건 패킷수신 실패로 정리 !!
  int                     nowEncoderCount = 1;              // Every encoder Status check. 1 - 4 , 2022.02.27

  // Timer Mode
  unsigned long           previousTimerTime = 0;            // Timer loop 를 위한 시간변수
  unsigned long           ledPreTimerTime = 0;              // Timer loop LED on 시간변수
  unsigned long           ledOnDelayPreTime = 0;            // [NEW] LED ON loop 시간변수.
  int                     ledTimerCount = 0;                // [NEW] LED ON COUNT
  int                     triggerTimerCount = 0;            // [NEW] TRIGGER ON COUNT 

  // Program sequnce (1 ~ 255)
  int                     programSequence = 1;              // Sequence 적용을 위한 변수
  byte                    programSequenceHex[2];            // Sequence 적용을 위한 변수

  
  // EGG EXIT
  byte                    exitEggValue =        0b00000000;
  byte                    exitBadEggValue =     0b00000000;
  

  // SERIAL COMM VARIABLE
  char                    toCheck[200];
  int                     bufSize = 0;

  // Loop count for all
  unsigned long           currentTime =       0;            // 전역변수로 변경 (2022.01.31)
  unsigned long           loopCount =         0;
  unsigned long           triggerCount =      0; 
  unsigned long           triggerOffCount =   0;
  unsigned long           triggerLoopCount =  0;
  unsigned long           eggExitDelayCount = 0;
  unsigned long           eggExitCount =      0;
  unsigned long           badExitDelayCount = 0; 
  unsigned long           badExitCount =      0;
  unsigned long           eggExitLoopCount =  0;
  unsigned long           badExitLoopCount =  0;
  unsigned long           ledOnCount =        0;
  unsigned long           ledOnDelayCount =   0;
  unsigned long           ledOnOffLoopCNT =   0;

  // COMMON LOADCELL ARRAY (LOADCELL_TOTAL)
  
  byte  eggBoardLoadCell[2][LOADCELL_TOTAL];
  int   appliedLoadCellCount    =  LOADCELL_TOTAL;          // 현재 장비의 Loadcell total count , [TODO] packet에 정리해서 장비별로 세팅 가능 하도록 해야 할 듯. (향후 늘어날 계획)
  
  // Env Settings object instance
  eggBoardEnv eggBoard;  

  unsigned long lastMsgTime = 0;

  // Interrupt related variables
  volatile bool g_homeEvent          = false;
  volatile bool g_stepBoundaryEvent  = false;


  char Test_Flag = 0 ;

    // PWM SERVO DRIVER
  // LEDBANK0 : LED 15 - 30
  // LEDBANK1 : LED 01 - 15
Adafruit_PWMServoDriver ledBank0 =  Adafruit_PWMServoDriver(0x40 , Wire1);    //A0 ~ A5 : GND 
Adafruit_PWMServoDriver ledBank1 =  Adafruit_PWMServoDriver(0x41 , Wire1);    //A0 : VCC , A1~A5 : GND

const uint8_t CRACK_PINS[8] = {
  CRACK_EXIT1, CRACK_EXIT2, CRACK_EXIT3, CRACK_EXIT4,
  CRACK_EXIT5, CRACK_EXIT6, CRACK_EXIT7, CRACK_EXIT8
};

const uint8_t BAD_PINS[8] = {
  BAD_EXIT1, BAD_EXIT2, BAD_EXIT3, BAD_EXIT4,
  BAD_EXIT5, BAD_EXIT6, BAD_EXIT7, BAD_EXIT8
};

const uint8_t TRIGGER_PINS[4] = { TRIGGER1, TRIGGER2, TRIGGER3, TRIGGER4 };
#endif // VARIABLE_EGG_BOARD_H