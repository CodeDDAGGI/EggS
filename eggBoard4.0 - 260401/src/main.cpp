#include <Arduino.h>
#include "config/VariableEggBoard.hpp"
#include "CameraControl/CameraControl.h"
#include "EggExit/EggExit.h"
#include "pinSetup/pinSetup.h"
#include "NearSensor/NearSensor.h"
#include "FlashROM/FlashROM.h"
#include "LEDControl/LEDControl.h"
#include "Interrupt/Interrupt.h"
#include "Serial/Serial.h"
#include "Utility/Utility.h"
    void setup() {
        Wire1.begin(); // 하부 LED 연결
        
        // Setup Serial Monitor , SerialUSB for debug
        SerialUSB.begin(115200); // USB3 NativeUSB port
        // Serial3.begin(115200); // SerialUSB for Camera Controlss
        Serial2.begin(115200); // SerialUSB for Camera Control
        Serial2.setTimeout(100);
        // Serial1.begin(115200); // Serial1 for LED Control
        // Serial.begin(115200);
    
        delay(4000);
        
        SerialUSB.println("boot");
        setupPins();
        
        attachInterrupt(B_Encoder, countPlusISR, RISING);
        attachInterrupt(inputRESET, homeResetISR, RISING);

  
        // SerialUSB.println test.
        SerialUSB.println(" Hanbat IoT AI Egg Looker V2.1 ");
        SerialUSB.println(" Started................................... ");  

      // PWM Driver Init !!
      ledBank0.begin();
      ledBank1.begin();
    
      ledBank0.setPWMFreq(PWM_FREQUENCY);       // This is the maximum PWM frequency
      ledBank1.setPWMFreq(PWM_FREQUENCY);       // This is the maximum PWM frequency

  
      // I2C SPEED SETTING    
      Wire1.setClock(400000);                    // 400 kHz

    //   if(Read_FlashData()) SerialUSB.println("Flash Read OK");
    //   else SerialUSB.print("Flash Read FAILED. INIT VALUE not settled ... (first time running..?)");

    //   getLedArray_FlashData();
      
      bool flashOK = Read_FlashData();
      SerialUSB.println("Flash Read OK");
    if (!flashOK) {
    //   if(runMode < 0){
          // For the first time
          runMode =           0;
          ledOnPosition =     0;
          ledBrightness =     2000;
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

          for (int i = 0; i < ledCount; i++) {
            ledEnvArrayInt[0][i] = ledBrightness;
            ledEnvArrayInt[1][i] = ledDuration;
         }

          Save_FlashData(); 
      }
      intArrayToByteArray();
      // LED SET
      setLEDarray(optionBits);

      // 무조건 모든 LED 끄고 시작. (Most of all, All led off !!!!)
      ledOff();

      
      // Load getLedArrayEEPROM
    //   getLedArray_FlashData();
      
    //   if(ledEnvArrayInt[0][0] <= 0 || ledEnvArrayInt[1][0] <= 0){
    //       // Data not settled. Init valued settle
    //       SerialUSB.println("ledArrayEEPROM not settled just INIT !! ");
    //       for(int i = 0; i< ledCount; i++ ) {
    //             ledEnvArrayInt[0][i] = ledBrightness;
    //             ledEnvArrayInt[1][i] = ledDuration;
    //       }
    //   } 
    //   else {

    //       // INT date change to byte array for communication !
    //     }

        
  }

void loop() {
  currentTime = millis();

    if (g_homeEvent) {
        // SerialUSB.print("Home Event ON");
        handleHomeEvent();
    }
    if (g_stepBoundaryEvent) {
        // SerialUSB.print("Step Boundary Event ON");
        handleStepBoundaryEvent();
    }
        
//         // For the first time checking modules here !!
        if(FirstRun) {
            I2C_finder();
            print_FlashData();
            FirstRun = false;   
            // SerialUSB.print("FirstRun");
        }
         


        // Serial port check always
        if (Serial2.available() > 0) {
            // Serial processing function
            // bool result = serialProcessing();
            serialProcessing();
        }

        
        // Encoder Mode Default START point !! 
        if(runMode == ENCODER_MODE && eggSTART == true && oneStepMove == true){
                      #ifdef DEBUG
                          SerialUSB.println(".... Main Logic Entered !!");
                      #endif
                                          
                      readNearSensor(); //Loop change location out of this loop. 

                      innerLoop = true;
                    
                  //  0. EggExist Settings on ARRAY. 
                      eggExistSetting();


                  //  1. T-0 : EggExist packet send & receive response.
                      syncPulse();
    
                  //  2. 3. LED ON , CAMERA TRIGGER 전달 (with delay & duration)
                      ledSTART = true;
                      

                      // ledON();   // [NEW] LED delay time 설정 하면서 밑에서 켜 줌. 
                     ledLight1 = eggBoardLoadCell[0][proximityStep+4];
                     ledLight2 = eggBoardLoadCell[0][proximityStep+3];
                     ledLight3 = eggBoardLoadCell[0][proximityStep+2];
                     ledLight4 = eggBoardLoadCell[0][proximityStep+1];
                     ledLight5 = eggBoardLoadCell[0][proximityStep+0];
                
                      if(ledLight1 != 0)    {
                        triggerSTART = true;
                        SerialUSB.println("Trigger Start1");
                        }      
                      if(ledLight2 != 0)    {
                        triggerSTART = true;
                        SerialUSB.println("Trigger Start2");
                        }
                      if(ledLight3 != 0)    {
                        triggerSTART = true;
                        SerialUSB.println("Trigger Start3");
                        }
                        if(ledLight4 != 0)    {
                        triggerSTART = true;
                        SerialUSB.println("Trigger Start4");
                        }
                        if(ledLight5 != 0)    { 
                        triggerSTART = true;
                        SerialUSB.println("Trigger Start5");
                        }

                     // if(ledLight1 != 0)     triggerSTART = true;
                    //   else if(ledLight2 != 0)     triggerSTART = true;
                    //   else if(ledLight3 != 0)     triggerSTART = true;
                    //   else if(ledLight4 != 0)     triggerSTART = true;
                    //   else if(ledLight5 != 0)     triggerSTART = true;

                      #ifdef DEBUG
                          // Trigger Starting point
                          if(triggerSTART) SerialUSB.println("Trigger Start");
                      #endif
                      
                  //  4. Receive CRACK RESULT (Flag processing) --> should be. 

    
                  //  5. Crack Egg exit !!
               
                      // proximityStep+3+ 번째 배열.

                      // Crack Exit Position 
                      // 판정결과 나오는 스텝이 0 임.
                      // Step 은 위치 카운트 (롤러), Position 은 설정 ms 이후에 on 시킴, Duration 은 시간만큼. 
                      int positionValue = proximityStep+3+loadCellStep;  // 변경 됨. 다시 적용 됨. 2021.12.05
    
                      exitEggValue = eggBoardLoadCell[0][positionValue];
    
                      if(exitEggValue != 0) {
                          eggExitSTART = true;

                          // Delay time 때문에 밑으로 이동해서 ON 시켜 줌.
                          // crackExitOn(exitEggValue);
                      }
    
                      #ifdef DEBUG_EXIT
                          SerialUSB.print("CRACK EXIT ");
                          SerialUSB.print("POSITION :[");
                          SerialUSB.print(positionValue);
                          SerialUSB.print("]");
                          SerialUSB.print(" VALUE :[");
                          SerialUSB.print(exitEggValue, BIN);
                          SerialUSB.println("]");
                      #endif 
                        
    
                  //  6. Bad Egg exit !!
                      // Step 은 위치 카운트 (롤러), Position 은 설정 ms 이후에 on 시킴, Duration 은 시간만큼.
                      positionValue = proximityStep+3+badOutputStep;  // 새롭게 적용 됨. 2021.12.05
                      
                      exitBadEggValue = eggBoardLoadCell[1][positionValue];
                     
                      if(exitBadEggValue != 0) {
                          eggBadExitSTART = true;

                          // Delay time 때문에 밑으로 이동해서 ON 시켜 줌.
                          // badExitOn(exitBadEggValue);
                      }

                      #ifdef DEBUG_EXIT
                          SerialUSB.print("BAD EXIT ");
                          SerialUSB.print("POSITION :[");
                          SerialUSB.print(positionValue+1);
                          SerialUSB.print("]");
                          SerialUSB.print(" VALUE :[");
                          SerialUSB.print(exitBadEggValue, BIN);
                          SerialUSB.println("]");
                      #endif 

    
                  //  8. Set loop again.
                      oneStepMove = false;
                      loopCount = 0;
          }  
          // (oneStepMove == true 조건 추가, 계란이 없어도, 엔코더는 연동해서 사진이 전부 찍히도록 처리 함 !!, 2022.01.20)
         else if(runMode == TIMER_MODE) {
              // TIMER MODE 이면서 timerSTART true 그리고 manualTimerPeriod != 0 
              if(timerSTART == true && manualTimerPeriod != 0) {
                
                    if(currentTime - previousTimerTime >= manualTimerPeriod) {

                            previousTimerTime = currentTime;
                            
                            // syncPulse 전달함 (전체 계란존재 0x00111111)
                            syncPulseTimer();

                            // led ON
                            
                            ledONTimer();

                            TriggerController::triggerOn();
          
                            //timer 등록
                            //timer 처리 (Duration 이후 OFF)
                            if(currentTime - ledPreTimerTime >= ledDuration) {
                                  ledTimerCount++;
                                  
                                  if(ledTimerCount >= 2) {
                                      // Turn OFF LED
                                      ledOffTimer();
                                      ledTimerCount = 0;
                                    
                                  } else ledPreTime = currentTime;
                            }
                            
                            if(currentTime - triggerOffPreTime >= triggerDuration) {
                                    triggerTimerCount++;

                                    if(triggerTimerCount >= 2) {
                                      
                                          TriggerController::triggerOff();
                                          triggerTimerCount = 0;
                                      
                                    }else triggerOffPreTime = currentTime;
                            }

                            
                            #ifdef DEBUG
                                SerialUSB.println("TRIGGER MODE ... ");
                            #endif
                            
                    }
              }
              
        } else if(runMode == MANUAL_MODE) {
        }
        



        // 2. LED ON / OFF 처리 ////////////////////////////////////////////////////////////////////////// 
        if(ledSTART == true){

            ledOnOffLoopCNT++;

            //eggExit Delay timer 적용
            if(currentTime - ledOnDelayPreTime >= ledOnPosition) {

                  ledOnDelayCount++;
                  
                  if(ledOnDelayCount == 2) {

                        // LED ON
                        ledON();

                        #ifdef DEBUG_LED
                            SerialUSB.print("LED ON DELAY ~ count:[");
                            SerialUSB.print(ledOnDelayCount);
                            SerialUSB.print("][");
                            SerialUSB.print(currentTime - ledOnDelayPreTime);
                            SerialUSB.print("][");
                            SerialUSB.print(ledOnPosition);
                            SerialUSB.print("][");
                            SerialUSB.print(ledOnOffLoopCNT);
                            SerialUSB.println("]");
                        #endif


                        // Each LED timestamp setting
                        for (int i = 0; i < ledCount ; i++) {
                              // 
                              ledPreTimeArray[i] = currentTime;
                        }

                        ledEachStart = true;

                        #ifdef DEBUG
                            SerialUSB.println("Each LED timestamp set completed !!!! ");
                        #endif
                        
                  }
                  ledOnDelayPreTime = currentTime;
            }
          

           

            // timer 처리 (개별 led duration time 처리 function call) 
            if(ledEachStart) ledEachOff();

        }
          
        //  3. CAMERA TRIGGER 전달 (with delay & duration) //////////////////////////////////////////////////////////////////////////
        if(triggerSTART == true) {

                triggerLoopCount++;

                //Delay timer 적용
                if(currentTime - triggerDelayPreTime >= triggerDelay) {

                      triggerCount++;
                      if(triggerCount == 2) {
                            TriggerController::triggerOn();

                            #ifdef DEBUG_TRIGGER
                                SerialUSB.print("Trigger ON ~ trigger delay:[");
                                SerialUSB.print(currentTime - triggerDelayPreTime);
                                SerialUSB.print("] trigger delay[");
                                SerialUSB.print(triggerDelay);
                                SerialUSB.println("]");
                            #endif
                      }
                      triggerDelayPreTime = currentTime;
                      
                }
                
                if(currentTime - triggerOffPreTime >= (triggerDelay + triggerDuration)) {
         
                        triggerOffCount++;
                        
                        if(triggerOffCount == 2) {
                          
                            triggerSTART = false;
                            
                            triggerCount =      0;
                            triggerOffCount =   0;
                            triggerLoopCount =  0;

                            TriggerController::triggerOff();
                            
                            #ifdef DEBUG_TRIGGER
                                SerialUSB.print("Trigger OFF ~ total duration :[");
                                SerialUSB.print(currentTime - triggerOffPreTime);
                                SerialUSB.print("] setting value [");
                                SerialUSB.print(triggerDelay + triggerDuration);
                                SerialUSB.println("]");
                            #endif
                        }
                        triggerOffPreTime = currentTime;  
                  }
        }
        
      if(eggExitSTART == true){

            // Step on Position delay
            if(oneStepCounter == outputPosition) {
                  EggExit::crackExitOn(exitEggValue);

                  #ifdef DEBUG_EXIT
                      SerialUSB.print("CRACK ON ~ onestep counter:[");
                      SerialUSB.print(oneStepCounter);
                      SerialUSB.print("] Output Position [");
                      SerialUSB.print(outputPosition);
                      SerialUSB.println("]");
                  #endif
            } else if (oneStepCounter == (outputPosition + outputDuration)) {
                  EggExit::crackExitOff(exitEggValue);

                  #ifdef DEBUG_EXIT
                      SerialUSB.print("CRACK OFF ~ onestep counter:[");
                      SerialUSB.print(oneStepCounter);
                      SerialUSB.print("] Output Position + outputDuration [");
                      SerialUSB.print(outputPosition + outputDuration);
                      SerialUSB.println("]");
                  #endif

                  eggExitSTART = false;
            }
        
      }
        
        // eggBadExit Timer processing.. ////////////////////////////////////////////////////////////////////////// 
        // Step based signal sending !! 2021.12.06
        if(eggBadExitSTART == true){

              // 정방향 READ MODULE
              // Read step count.
              if(oneStepCounter >= 0 && oneStepCounter <= 24) {
                  // onPosition step 
                  if(oneStepCounter == (0 + badOutputPosition) ){
                      if(exitOrderASC == true) {
                            // 0번째 eggExitOn
                            //void badExitOnOne(byte data, int count) 
                            EggExit::badExitOnOne(exitBadEggValue, 0);
      
                            //badExitOn(exitBadEggValue);
                            #ifdef DEBUG_EXIT
                                SerialUSB.print("BAD ON ~ [0] oneStepCounter [");
                                SerialUSB.print(oneStepCounter);
                                SerialUSB.print("] badOutputPosition [");
                                SerialUSB.print(badOutputPosition);
                                SerialUSB.println("]");
                            #endif
                      } else {
                            // 5번째 eggExitOn
                            //void badExitOnOne(byte data, int count) 
                            EggExit::badExitOnOne(exitBadEggValue, 5);
      
                            //badExitOn(exitBadEggValue);
                            #ifdef DEBUG_EXIT
                                SerialUSB.print("BAD ON ~ [5] oneStepCounter [");
                                SerialUSB.print(oneStepCounter);
                                SerialUSB.print("] badOutputPosition [");
                                SerialUSB.print(badOutputPosition);
                                SerialUSB.println("]");
                            #endif
                      }

                  }

                  if(oneStepCounter == (0 + badOutputPosition + badOutputDuration)) {
                      // 0번째 eggExitOff
                      EggExit::badExitOffOne();

                      //badExitOff(exitBadEggValue);

                      #ifdef DEBUG_EXIT
                          SerialUSB.print("BAD OFF ~ [0][5] oneStepCounter [");
                          SerialUSB.print(oneStepCounter);
                          SerialUSB.print("] badOutputDuration [");
                          SerialUSB.print(0 + badOutputPosition + badOutputDuration);
                          SerialUSB.println("]");
                      #endif
                  }
              } else if(oneStepCounter >= 25 && oneStepCounter <= 49) {
                  // onPosition step 
                  if(oneStepCounter == (25 + badOutputPosition) ){
                      if(exitOrderASC == true) {
                          // 1번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 1);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [1] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] badOutputPosition [");
                              SerialUSB.print(25 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                      } else {
                          // 1번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 4);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [4] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] badOutputPosition [");
                              SerialUSB.print(25 + badOutputPosition);
                              SerialUSB.println("]");b
                          #endif
                      }
                      
                  }

                  if(oneStepCounter == (25 + badOutputPosition + badOutputDuration)) {
                      // 1번째 eggExitOff
                      EggExit::badExitOffOne();

                      #ifdef DEBUG_EXIT
                          SerialUSB.print("BAD OFF ~ [1][4] oneStepCounter [");
                          SerialUSB.print(oneStepCounter);
                          SerialUSB.print("] badOutputDuration [");
                          SerialUSB.print(25 + badOutputPosition + badOutputDuration);
                          SerialUSB.println("]");
                      #endif
                  }
                
              } else if(oneStepCounter >= 50 && oneStepCounter <= 74) {
                  // onPosition step 
                  if(oneStepCounter == (50 + badOutputPosition) ){
                      if(exitOrderASC == true) {
                          // 2번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 2);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [2] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] badOutputPosition [");
                              SerialUSB.print(50 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                      } else {
                          // 3번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 3);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [3] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] badOutputPosition [");
                              SerialUSB.print(50 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                      }
                      
                  }

                  if(oneStepCounter == (50 + badOutputPosition + badOutputDuration)) {
                      // 2번째 eggExitOff
                      EggExit::badExitOffOne();

                      #ifdef DEBUG_EXIT
                          SerialUSB.print("BAD OFF ~ [2][3] oneStepCounter [");
                          SerialUSB.print(oneStepCounter);
                          SerialUSB.print("] badOutputDuration [");
                          SerialUSB.print(50 + badOutputPosition + badOutputDuration);
                          SerialUSB.println("]");
                      #endif
                  }
                
              } else if(oneStepCounter >= 75 && oneStepCounter <= 99) {
                  // onPosition step 
                  if(oneStepCounter == (75 + badOutputPosition) ){
                      if(exitOrderASC == true) {
                          // 3번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 3);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [3] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] badOutputPosition [");
                              SerialUSB.print(50 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                        
                      } else {
                          // 2번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 2);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [2] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] badOutputPosition [");
                              SerialUSB.print(50 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                      }
                      
                  }

                  if(oneStepCounter == (75 + badOutputPosition + badOutputDuration)) {
                      // 3번째 eggExitOff
                      EggExit::badExitOffOne();

                      #ifdef DEBUG_EXIT
                          SerialUSB.print("BAD OFF ~ [3][2] oneStepCounter [");
                          SerialUSB.print(oneStepCounter);
                          SerialUSB.print("] badOutputDuration [");
                          SerialUSB.print(75 + badOutputPosition + badOutputDuration);
                          SerialUSB.println("]");
                      #endif
                  }
                
              } else if(oneStepCounter >= 100 && oneStepCounter <= 124) {

                  if(exitOrderASC == true) {
                      // onPosition step 
                      if(oneStepCounter == (100 + badOutputPosition) ){
                          // 4번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 4);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [4] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] badOutputPosition [");
                              SerialUSB.print(100 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                        
                      }
                  } else {
                      // onPosition step 
                      if(oneStepCounter == (100 + badOutputPosition) ){
                          // 1번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 1);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [1] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] badOutputPosition [");
                              SerialUSB.print(100 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                      }
                  }

                  if(oneStepCounter == (100 + badOutputPosition + badOutputDuration)) {
                      // 4번째 eggExitOff
                      EggExit::badExitOffOne();

                      #ifdef DEBUG_EXIT
                          SerialUSB.print("BAD OFF ~ [4][1] oneStepCounter [");
                          SerialUSB.print(oneStepCounter);
                          SerialUSB.print("] badOutputDuration [");
                          SerialUSB.print(100 + badOutputPosition + badOutputDuration);
                          SerialUSB.println("]");
                      #endif
                  }
                
              } else if(oneStepCounter >= 125 && oneStepCounter <= 149) {

                  if(exitOrderASC == true) {
                      // onPosition step 
                      if(oneStepCounter == (125 + badOutputPosition) ){
                          // 5번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 5);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [5] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] 125 + badOutputPosition [");
                              SerialUSB.print(125 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                      }
                  } else {
                      // onPosition step 
                      if(oneStepCounter == (125 + badOutputPosition) ){
                          // 0번째 eggExitOn
                          //void badExitOnOne(byte data, int count) 
                          EggExit::badExitOnOne(exitBadEggValue, 0);
    
                          #ifdef DEBUG_EXIT
                              SerialUSB.print("BAD ON ~ [0] oneStepCounter [");
                              SerialUSB.print(oneStepCounter);
                              SerialUSB.print("] 125 + badOutputPosition [");
                              SerialUSB.print(125 + badOutputPosition);
                              SerialUSB.println("]");
                          #endif
                      }
                  }
                      

                  if(oneStepCounter == (125 + badOutputPosition + badOutputDuration)) {
                      // 0번째 eggExitOff
                      EggExit::badExitOffOne();

                      #ifdef DEBUG_EXIT
                          SerialUSB.print("BAD OFF ~ [5][0] oneStepCounter [");
                          SerialUSB.print(oneStepCounter);
                          SerialUSB.print("] 125 + badOutputPosition + badOutputDuration [");
                          SerialUSB.print(125 + badOutputPosition + badOutputDuration);
                          SerialUSB.println("]");
                      #endif

                      // 최종 마무리 완료 !! ==> Not Here !!  2022.03.07. 
                      // eggBadExitSTART = false;
                  }
              }
        }

            
      //최종 Exit 
      if(innerLoop == true && eggBadExitSTART == false && eggExitSTART == false && triggerSTART == false && ledSTART == false){
            //safely shift array now() !!
            
            #ifdef DEBUG
              SerialUSB.println("One cycle logic finished");
            #endif
            SerialUSB.println("NORMAL CHECK");
            shiftLoadCellArray();
            innerLoop = false;
      }


      
      loopCount++;
  }