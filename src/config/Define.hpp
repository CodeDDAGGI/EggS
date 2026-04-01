#include <Arduino.h> 
#define DEBUG_INTERRUPT

  //ENCODER PIN mapping
  #define B_Encoder          52        // B  Every Encoder step count
  #define inputRESET    30        // Z  HOME position indicator
  
  // // TRIGGER PIN (for camera)
  // #define   PLC1    25
  // #define   PLC2    26
  // #define   PLC3    27
  // #define   PLC4    28


  // EXTERNAL INDICATOR PINs (LED for indication) V2.1
  // #define EXTLED1       28       
  // #define EXTLED2       30
  // #define EXTLED3       31

  //BAD EXIT PIN DEFINE.
  #define CRACK_EXIT1 33
  #define CRACK_EXIT2 34
  #define CRACK_EXIT3 35
  #define CRACK_EXIT4 36
  #define CRACK_EXIT5 37
  #define CRACK_EXIT6 38
  #define CRACK_EXIT7 39
  #define CRACK_EXIT8 40

  //Egg near sesors   
  #define NEAR_SENSOR1   43
  #define NEAR_SENSOR2   44
  #define NEAR_SENSOR3   45
  #define NEAR_SENSOR4   46
  #define NEAR_SENSOR5   6
  #define NEAR_SENSOR6   7
  #define NEAR_SENSOR7   8
  #define NEAR_SENSOR8   9

  //LED PWM Frequency
  #define PWM_FREQUENCY       1000    // 1KHz
  
  //RUN MODE
  #define ENCODER_MODE        0
  #define TIMER_MODE          1
  #define MANUAL_MODE         2

  // LED ARRAY
  // default LED light counter
  #define       ledCount    30       

  //Timer values set
  #define nearSensorInterval          2000     // milisec
  #define nearSensorPacketInterval    4000     // milisec

  // 없애도 되는 부분분
  // Test PIN
#define TEST_PIN 22      //PB26, PWM13

// BAD EXIT PIN DEFINE.
// BAD SPARE PIN - TESTPIN
#define BAD_EXIT1 94 // 46
#define BAD_EXIT2 95 // 45
#define BAD_EXIT3 96 // 44
#define BAD_EXIT4 97 // 94
#define BAD_EXIT5 98 // 9
#define BAD_EXIT6 99 // 8
#define BAD_EXIT7 100 // 7
#define BAD_EXIT8 101 // 6
  
// LED CONTROL PIN
#define LED_EXCLK 54      //PA16, S_EXCLK
#define LED_OE0 24      //PA15, S_OE00
#define LED_OE1 23      //PA14, S_OE01

#define TRIGGER1 25
#define TRIGGER2 26
#define TRIGGER3 27
#define TRIGGER4 28

// 추가 카메라
// #define TRIGGER5 29
// #define TRIGGER6 30
// #define TRIGGER7 31
// #define TRIGGER8 32
