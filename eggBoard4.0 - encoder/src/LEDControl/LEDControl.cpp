#include "LEDControl.h"
#include "config/VariableEggBoard.hpp"
#include "CameraControl/CameraControl.h"
  /**
   * LEDControl.cpp
   * 
   * agr.pj
   * 
   * 2025.04.22
   */

  //For the first time find out and print I2C devices
  void I2C_finder()
  {
      int nDevices = 0;
  
      SerialUSB.println("I2C Scanning...");
      
      for (byte address = 1; address < 127; ++address) {
          Wire1.beginTransmission(address);
          byte error = Wire1.endTransmission();
      
          if (error == 0) {
                SerialUSB.print("I2C device found at address 0x");
              if (address < 16) {
                SerialUSB.print("0");
              }
              SerialUSB.print(address, HEX);
              SerialUSB.println("  !");
        
              ++nDevices;
          } else if (error == 4) {
            SerialUSB.print("Unknown error at address 0x");
              if (address < 16) {
                SerialUSB.print("0");
              }
              SerialUSB.println(address, HEX);
          }
      }
      
      if (nDevices == 0) {
        SerialUSB.println("No I2C devices found\n");
      } else {
        SerialUSB.println("done\n");
      }
  }
  
    // LED ON for I2C comm.
    void processLED(byte ledBits, int startIndex) {
        int bitValue = 0;

        for (uint8_t pwmnum = 0; pwmnum < 6; pwmnum++) {
            bitValue = bitRead(ledBits, pwmnum);
            if (bitValue) {
                int pwmIndex = startIndex + pwmnum;
                int tmpVal = ledPinArray[0][pwmIndex];
                ledBrightness = ledEnvArrayInt[0][tmpVal];

                SerialUSB.print("bitValue : "); SerialUSB.println(bitValue);
                SerialUSB.print("tmpVal : "); SerialUSB.println(tmpVal);
                SerialUSB.print("ledBrightness : "); SerialUSB.println(ledBrightness);
                
                // ledBank0의 채널을 15개 사용
                // ledBank1의 채널을 9개 사용
                if (tmpVal >= 15) {
                    ledBank1.setPWM(tmpVal - 15, 0, ledBrightness);
                } else {
                    ledBank0.setPWM(tmpVal, 0, ledBrightness);
                }

                #ifdef DEBUG_LED_BRIGHT
                    SerialUSB.print(tmpVal);
                    SerialUSB.print(tmpVal >= 15 ? "]1[" : "]0[");
                    SerialUSB.print(ledBrightness);
                    SerialUSB.println("]");
                #endif
            }
        }
    }
    void ledON() {
        // 순서 주의: proximityStep + 3부터 0까지가 열의 첫 줄 ~ 네 번째 줄
        byte ledLights[5] = {
            eggBoardLoadCell[0][proximityStep + 4],
            eggBoardLoadCell[0][proximityStep + 3],
            eggBoardLoadCell[0][proximityStep + 2],
            eggBoardLoadCell[0][proximityStep + 1],
            eggBoardLoadCell[0][proximityStep + 0]
        };
        
        // 4줄을 반복
        for (int i = 0; i < 5; ++i) {
            processLED(ledLights[i], i * 6);
        }

        #ifdef DEBUG_LED
            SerialUSB.print("LED ON VARs ");
            for (int i = 0; i < 4; i++) {
                SerialUSB.print(" ");
                SerialUSB.print(i + 1);
                SerialUSB.print(" :[");
                SerialUSB.print(ledLights[i], BIN);
                SerialUSB.print("]");
            }
            SerialUSB.println();
        #endif
    }

  void ledOff(){

         // LED 무조건 꺼짐
         // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
         // 0 1 2 3 4 5 6 7 8 9 19 11  0  1  2  3  4  5  6  7  8  9 10 11
         // bank0                      bank1
 

         for (int pwmnum = 0; pwmnum < ledCount; pwmnum++) {
                int tmpVal = ledPinArray[0][pwmnum];

                if(tmpVal >= 15) {
                    tmpVal = tmpVal - 15;
                    ledBank1.setPWM(tmpVal, 0, 0);
                }else{
                    ledBank0.setPWM(tmpVal, 0, 0);
                }
                
        }
    
  }

  // LED 개별 제어 OFF (Each duration 적용)
  void ledEachOff() {
    
        // 무조건 Loop 돌면서 모든 LED duration time 지났는지 확인 (LED ON 시점에 모든 ledPreTimeArray[30] 에 timestamp setting 완료 되어 있음)

        // 무조건 모든 LED 꺼져야 됨. 2022.03.17 BUG FIX. END !!!!!
        nowLedOffCount = 0;
        
        for(int i=0 ; i<ledCount; i++) {
            // 모든 LED timestamp 확인. ledPreTimeArray // 시간은 이 배열에 들어가 있음.
            if(currentTime - ledPreTimeArray[i] >= ledEnvArrayInt[1][i]) { 

                  int tmpVal = ledPinArray[0][i];

                  if(tmpVal >= 15) {
                      tmpVal = tmpVal - 15;
                      ledBank1.setPWM(tmpVal, 0, 0);

                      // Count 위치 옮김. OFF 작동시에만 nowLedOffCount++ 해 줌.(2022.02.21)
                      nowLedOffCount++;

                      #ifdef EACH_LED_DEBUG
                          SerialUSB.print("NUMBER :[");
                          SerialUSB.print(ledPinArray[0][i]);
                          SerialUSB.print("] DURATION :[");
                          SerialUSB.print(ledEnvArrayInt[1][i]);
                          SerialUSB.print("] NOW INTERVAL :[");
                          SerialUSB.print(currentTime - ledPreTimeArray[i]);
                          SerialUSB.print("] LOOP COUNT :[");
                          SerialUSB.print(ledOnOffLoopCNT);
                          SerialUSB.println("]");
                      #endif
                  } else {
                      ledBank0.setPWM(tmpVal, 0, 0);

                      // Count 위치 옮김. OFF 작동시에만 nowLedOffCount++ 해 줌.(2022.02.21)
                      nowLedOffCount++;
                      
                      #ifdef EACH_LED_DEBUG
                          SerialUSB.print("NUMBER :[");
                          SerialUSB.print(ledPinArray[0][i]);
                          SerialUSB.print("] DURATION :[");
                          SerialUSB.print(ledEnvArrayInt[1][i]);
                          SerialUSB.print("] NOW INTERVAL :[");
                          SerialUSB.print(currentTime - ledPreTimeArray[i]);
                          SerialUSB.print("] LOOP COUNT :[");
                          SerialUSB.print(ledOnOffLoopCNT);
                          SerialUSB.println("]");
                      #endif
                  }
            }
        }
        #ifdef EACH_LED_DEBUG
            SerialUSB.print("NOW OFF TOTAL COUNT : [");
            SerialUSB.print(nowLedOffCount);
            SerialUSB.print("]");
        #endif

        // Whole the LED off time
        if(nowLedOffCount >= ledCount) {

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
  }

  // LED ON for I2C comm.
  // LED bright change ledBrightLocal from ledBrightness
  void ledManualON(byte ledLight1, byte ledLight2, byte ledLight3, byte ledLight4 , byte ledLight5)
  {
    handleManualLED(ledLight1,  0 , true);  // 0~5
    handleManualLED(ledLight2,  8 , true);  // 6~11
    handleManualLED(ledLight3, 16 , true);  // 12~17
    handleManualLED(ledLight4, 24 , true); // 18~23 

    // LED 12 ON
    ledBank0.setPWM(15, 0, ledBrightness); // ledbank0 끝번 led 24개 기준
    ledBank1.setPWM(15, 0, ledBrightness); // ledbank1 끝번 led 24개 기준
    
    ledSTART = true;
  }

  // ledManualOFf by manual mode
  void ledManualOff(byte ledLight1, byte ledLight2, byte ledLight3, byte ledLight4 , byte ledLight5){
      handleManualLED(ledLight1,  0 , false);  // 0~5
      handleManualLED(ledLight2,  8 , false);  // 6~11
      handleManualLED(ledLight3, 16 , false);  // 12~17
      handleManualLED(ledLight4, 24 , false); // 18~23 
 

      // LED 12 ON
      ledBank0.setPWM(15, 0, 0); // ledbank0 끝번 led 24개 기준
      ledBank1.setPWM(15, 0, 0); // ledbank1 끝번 led 24개 기준
  }

  void handleManualLED(byte ledBits , int Index , bool OnOff) {
        for (uint8_t i = 0; i < 8; i++) {
        int bitValue = bitRead(ledBits, i);
        if (!bitValue) continue;

        int pinIndex = Index + i;

        // 안전: 혹시라도 잘못된 startIndex 들어와도 보호
        if (pinIndex < 0 || pinIndex >= ledCount) {
            continue;
        }

        int tmpVal = ledPinArray[0][pinIndex];

        // 밝기 계산 (processLED와 동일한 방식)
        int duty = OnOff ? (ledEnvArrayInt[0][tmpVal]) : 0;

        if (tmpVal >= 15) {
            ledBank1.setPWM(tmpVal - 15, 0, duty);
            SerialUSB.print(tmpVal - 15);
            SerialUSB.print("]1[");
            SerialUSB.print(duty);
            SerialUSB.println("]");
        } else {
            ledBank0.setPWM(tmpVal, 0, duty);
            SerialUSB.print(tmpVal);
            SerialUSB.print("]0[");
            SerialUSB.print(duty);
            SerialUSB.println("]");
        }
    }

    // for(uint8_t i = 0; i < 6; i++){
    //     int bitValue = bitRead(ledBits, i);
    //     if(bitValue){
    //         int pinIndex = Index + i;
    //         int tmpVal = ledPinArray[0][pinIndex];

    //         int OnVal = OnOff ? ledLightValue - ledEnvArrayInt[0][tmpVal] : 0;
    //         int OffVal = OnOff ? 0 : ledLightValue - ledEnvArrayInt[0][tmpVal];

    //         if (tmpVal >= 15) {
    //             ledBank1.setPWM(tmpVal - 15, OnVal, OffVal);
    //             SerialUSB.print(tmpVal - 15);
    //             SerialUSB.print("]1[");
    //             SerialUSB.print(ledBrightness);
    //             SerialUSB.println("]");
    //         } else {
    //             ledBank0.setPWM(tmpVal, OnVal, OffVal);
    //             SerialUSB.print(tmpVal);
    //             SerialUSB.print("]0[");
    //             SerialUSB.print(ledBrightness);
    //             SerialUSB.println("]");
    //         }
    //     }
    // }
  }

  // LED ON for I2C comm.
  // Changed LED brightness ledBrightLocal from ledBrightness
  void ledONTimer(){

        // LED BANK0
        for (int pwmnum = 0; pwmnum < 15; pwmnum++) {
                int tmpVal = ledPinArray[0][pwmnum];
                ledBrightness =  ledEnvArrayInt[0][tmpVal];

                if(tmpVal >= 15) {
                    tmpVal = tmpVal - 15;
                    ledBank1.setPWM(tmpVal, 0, ledBrightness);
                } else {
                    ledBank0.setPWM(tmpVal, 0, ledBrightness);
                } 
        }
        // LED BANK1
        for (int pwmnum = 0; pwmnum < 15; pwmnum++) {
                int tmpVal = ledPinArray[0][pwmnum+13];
                ledBrightness =  ledEnvArrayInt[0][tmpVal];

                if(tmpVal >= 15) {
                    tmpVal = tmpVal - 15;
                    ledBank1.setPWM(tmpVal, 0 , ledBrightness);
                } else {
                    ledBank0.setPWM(tmpVal, 0 , ledBrightness);
                }
        }
        // LED 12 ON

        ledBank0.setPWM(12, 0 , 4095);
        ledBank1.setPWM(12, 0 , 4095);
    
  }

  // Timer mode led off function
  void ledOffTimer(){

        for (int pwmnum = 0; pwmnum < 15; pwmnum++) {
                int tmpVal = ledPinArray[0][pwmnum];

                if(tmpVal >= 15) {
                    tmpVal = tmpVal - 15;
                    ledBank1.setPWM(tmpVal, 0, 0);
                }else{
                    ledBank0.setPWM(tmpVal, 0, 0);
                }
                
        }

        for (int pwmnum = 0; pwmnum < 15; pwmnum++) {
                int tmpVal = ledPinArray[0][pwmnum+13];

                if(tmpVal >= 15) {
                    tmpVal = tmpVal - 15;
                    ledBank1.setPWM(tmpVal, 0, 0);
                }else{
                    ledBank0.setPWM(tmpVal, 0, 0);
                }
        }

        // LED 12 OFF
        ledBank0.setPWM(12, 0, 0);
        ledBank1.setPWM(12, 0, 0);
    
  }

  // Each bright list print
  void ledBrightList(){
        SerialUSB.print("BRIGHTNESS LIST : ");
        for(int i = 0 ; i < ledCount ; i++) {
              SerialUSB.print("[");
              SerialUSB.print(ledEnvArrayInt[0][i]);
              SerialUSB.print("]");
        }
        SerialUSB.println("");
  }

  // Each duration print
  void ledDurationList(){
        SerialUSB.print("DRATION LIST : ");
        for(int i = 0 ; i < ledCount ; i++) {
              SerialUSB.print("[");
              SerialUSB.print(ledEnvArrayInt[1][i]);
              SerialUSB.print("]");
        }
        SerialUSB.println("");
  }


  // Set whole led param brightness
  void ledSetBright(int value){
    
        SerialUSB.print("SET BRIGHTNESS : ");
        for(int i = 0 ; i < ledCount ; i++) {
              ledEnvArrayInt[0][i] = value;

        }
        SerialUSB.print("[");
        SerialUSB.print(value);
        SerialUSB.println("]");
  }


  
  // SET ledPinArray[30]. For led settings
  // led 배열 세팅
  void setLEDarray(int type){    
      // PWM BANK ADDRESS (Not changed...Just 1time setting)
      for (int i = 0; i < ledCount; i++) {
        ledPinArray[1][i] = i % 12;  
      }
          
      if(type == 0) { // 정방향
          for(int i = 0; i< ledCount; i++) {
              ledPinArray[0][i] = i;
          }
        
      } else if(type == 1) { // 역방향
        for (int i = 0; i < 6; i++) {
            ledPinArray[0][i]      = 5 - i;         
            ledPinArray[0][i + 6]  = 11 - i;      
            ledPinArray[0][i + 12] = 17 - i;       
            ledPinArray[0][i + 18] = 23 - i;            
            ledPinArray[0][i + 24] = 29 - i;            
        }
            
      } else if(type == 2) {
        for(int i = 0; i < ledCount; i++){
            ledPinArray[0][i] = ledCount - 1 - i;
        } 
        
      } else if(type == 3) {
        for (int i = 0; i < 6; i++) {
            ledPinArray[0][i]      = 24 + i;      
            ledPinArray[0][i + 6]  = 18 + i;      
            ledPinArray[0][i + 12] = 12 + i;       
            ledPinArray[0][i + 18] = 6 + i;       
            ledPinArray[0][i + 24] = 0 + i;       
        }
      }

      SerialUSB.println("OPTION SET");

      for(int i = 0; i< ledCount; i++){
        SerialUSB.print("[");
        SerialUSB.print(ledPinArray[0][i]);
        SerialUSB.print("]");
        if((i+1) % 6 == 0) SerialUSB.println("");
      }
      
  }
