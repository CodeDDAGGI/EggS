#include "Utility/Utility.h"
#include "config/VariableEggBoard.hpp"
#include "config/Define.hpp"
#include "LEDControl/LEDControl.h"
#include <sam.h>
#include "FlashROM/FlashROM.h"
    
  /**
   * Utility.cpp
   * 
   * agr.pj
   * 
   * 2025.04.22
   */
    // Inter to Bytes convert function
    void integerToBytes(long val, byte b[2]){
        b[0] = (byte )((val >> 8) & 0xff);
        b[1] = (byte )(val & 0xff);
    }

    // EEPROM DATA TO byteArray for Serial communication (NEW !!, 2022.01.31)
    void intArrayToByteArray() {
        
        byte  tempValue[2];
        int   tempBright =      0;
        int   tempDuration =    0;
        int   byteBrightInt =   0;
        int   byteDurationInt = 0;

        // Read Int Array and set byteArray. Brightness !!
        for(int i=0; i<ledCount; i++) {
            // Each value change to byte array
            tempBright = ledEnvArrayInt[0][i];
            integerToBytes(tempBright, tempValue);

            // Mapping to ledEnvArray
            ledEnvArray[0][byteBrightInt] = tempValue[0];
            byteBrightInt++;
            ledEnvArray[0][byteBrightInt] = tempValue[1];
            byteBrightInt++;
        }

        // Read Int Array and set byteArray. Duration !!
        for(int j=0; j<ledCount; j++) {
            // Each value change to byte array
            tempDuration = ledEnvArrayInt[1][j];
            integerToBytes(tempDuration, tempValue);

            // Mapping to ledEnvArray
            ledEnvArray[1][byteDurationInt] = tempValue[0];
            byteDurationInt++;
            ledEnvArray[1][byteDurationInt] = tempValue[1];
            byteDurationInt++;
        }

        SerialUSB.println("byte ledEnvArray detail");
        SerialUSB.print("Brightness[");
        for(int i = 0; i < 60; i++) {
            
            SerialUSB.print(ledEnvArray[0][i], HEX);
            SerialUSB.print(",");
        }
        SerialUSB.println("]");
        
        SerialUSB.print("Duration[");
        for(int i = 0; i < 60; i++) {
            
            SerialUSB.print(ledEnvArray[1][i], HEX);
            SerialUSB.print(",");
        }
        SerialUSB.println("]");
        
    }
  
    //  Bytes to Integer. check and change values to led Array
    //  byte          ledEnvArray[2][48];           // 0:brightness, 1:duration
    //  int           ledEnvArrayInt[2][24];        // 0:brightness, 1:duration [int value changed array for handling process]
    void bytesToIntegerArray() {
      byte  bright[2];
      byte  duration[2];
      int   brightCount = 0;
      int   durationCount = 0;
      
      // Critical loop
      for(int i = 0; i < 60; i++){
        
          //brightness processing
          bright[i%2] = ledEnvArray[0][i];
          if(i%2 == 1){
              int nowBrightness = bright[0]*256 + bright[1];
              ledEnvArrayInt[0][brightCount] = nowBrightness;
              brightCount++;
          }
  
          //duration processing
          duration[i%2] = ledEnvArray[1][i];
          if(i%2 == 1){
              int nowDuration = duration[0]*256 + duration[1];
              ledEnvArrayInt[1][durationCount] = nowDuration;
              durationCount++;
          }
      }
  
      SerialUSB.println("ledEnvArrayInt detail");
      for(int i = 0; i < ledCount; i++){
          SerialUSB.print("[");
          SerialUSB.print(ledEnvArrayInt[0][i]);
          SerialUSB.print(",");
          SerialUSB.print(ledEnvArrayInt[1][i]);
          SerialUSB.print("]");
      }
      
    }
  
    // 통신을 위한 Sequence 를 지속적으로 반복 생성 함.
    // 1 - 255 sequence obtain. also 255 --> 1 loop.
    void getNowSequence(){
      
        // 전체 시스템의 sequence를 증가 함. (1~255)
        if(programSequence >= 255) programSequence = 1;
        else programSequence++;
        
        integerToBytes(programSequence, programSequenceHex);
    }
  
    // LoadCell data SHIFT function
    void shiftLoadCellArray(){
                        
         // AI decision packet not received, marking normal egg status !!! (2022.01.29)
         if(extBadPacketRcv == false) {
              // exit, bad sector init !

              eggBoardLoadCell[0][proximityStep+4] = 0x00;
              eggBoardLoadCell[1][proximityStep+4] = 0x00;
              
          } else SerialUSB.println("Packet Received");

          // extBadPacketRcv 변수는 여기서만 정리 함.
          extBadPacketRcv = false;

          //검증용 RESULT 출력하고 나서 SHIFT (2021.12.04. ARNOLD)
          #ifdef DEBUG_ARRAY
              for(int i=0; i<appliedLoadCellCount; i++){
                    SerialUSB.print("[");
                    SerialUSB.print(eggBoardLoadCell[0][i]);
                    SerialUSB.print("]");
              }
              SerialUSB.println("");
              for(int i=0; i<appliedLoadCellCount; i++){
                    SerialUSB.print("[");
                    SerialUSB.print(eggBoardLoadCell[1][i]);
                    SerialUSB.print("]");
              }
              SerialUSB.println("");
          #endif 
          
  
          // 유효한 사이즈 만큼만 사용 함. (MAX:50, DEFAULT:26)
          for(int i = appliedLoadCellCount-1; i > 0; i--){
                 eggBoardLoadCell[0][i] =  eggBoardLoadCell[0][i-1];
                 eggBoardLoadCell[1][i] =  eggBoardLoadCell[1][i-1];
          }
          eggBoardLoadCell[0][0] =  0x00;
          eggBoardLoadCell[1][0] =  0x00;


          // Debug LEDSERIAL
          #ifdef DEBUG_LED_BRIGHT
            ledBrightList();
          #endif


          // LED DURATION 에서 최종 마무리 (24 LED OFF) 강제 소등 - 전체
          // Whole the LED off time
          if(nowLedOffCount >= ledCount) {

             // 무조건 LED OFF
             ledOff();
  
              // LED VALUES INIT()
              ledSTART =        false;
              ledOnOffLoopCNT = 0;
              ledOnDelayCount = 0;
              nowLedOffCount =  0;
              ledEachStart =    false;
  
              // ledOnCount = 0; // NOT USED NOW !!
  
              #ifdef EACH_LED_DEBUG
                  SerialUSB.println("LED OFF FINISHED. LED INIT()");
              #endif
          }

          #ifdef DEBUG_LED_DURATION
              ledDurationList();
          #endif
    }


    // Reboot Function
    void reboot() {
        // // 1. Watchdog 활성화 - 약 15ms 후 리셋되도록 설정
        // WDT->WDT_MR = WDT_MR_WDRSTEN | WDT_MR_WDV(15);  // 15은 대략 15ms와 유사한 개념이지만 정확히 15ms는 아님
    
        // // 2. 무한 루프를 돌며 WDT 리셋 대기
        // while (1) {}
        // 3. 시스템 리셋
            runMode =           0;
          ledOnPosition =     0;
          ledBrightness =     2047;
          ledDuration =       25;
          triggerDelay =      5;
          triggerDuration =   10;
          optionBits =        3;
          proximityStep =     4;
          loadCellStep =      15;
          outputPosition =    0;
          outputDuration    = 10;
          badOutputStep     = 16;
          badOutputPosition = 0;
          badOutputDuration = 10;
          homePlus =           0;

          Save_FlashData(); 
    }

    // GPIO handling 
    void extLedOn() {
        // digitalWrite(EXTLED1, HIGH);
    }
    // GPIO handling 
    void extLedOff() {
        // digitalWrite(EXTLED1, LOW);
    }
