// #include "Interrupt/Interrupt.h"
// #include "config/VariableEggBoard.hpp"
// #include "config/Define.hpp"
// #include "Utility/Utility.h"
// #include "EggExit/EggExit.h"
// #include "NearSensor/NearSensor.h"
// #include "LEDControl/LEDControl.h"



// void homeReset()
//   {
      
//     // cli(); //We pause interrupts happening before we read pin values
  
//     homePosition  =       digitalRead(inputRESET);

//     // HOME PULSE INTERRUPT OCCURRED !!
//     if(homePosition == HIGH) {
//           pulseHOMECounter = 600;
//     }
    
//     // if(homePosition == HIGH) {  
//     if(nowHOMEposition) {        

//               // Control flag set.
//               nowHOMEposition = false;
//               pulseHOMECounter = 0;
              
//               #ifdef DEBUG_INTERRUPT
//                 SerialUSB.print("HOME [");
//                 SerialUSB.print(b_counter);
//                 SerialUSB.println("]");
//               #endif
              
//               b_counter = 0;

//               // LED ON during 200ms -- 외부 Indication LED 임. 밑에 Timer 이용해서 꺼줌. (2022.10)
//               ledON();
              

//               // First Starting point setup (2021.12.18)
//               // eggSTART가 true 가 아닌상태에서 최초 HOME(b_counter==600) 이면 최초 처리 함. 
//               if(eggSTART == false && runMode == ENCODER_MODE) {
                
//                   // This is start point
//                   eggSTART = true;
//                   oneStepMove = true;

//                   oneStepCounter = 0;

//                   #ifdef DEBUG_INTERRUPT
//                     SerialUSB.println("Home position. This is encoder start point. Start settled !!!!!");
//                   #endif

//                   // For the first time set 
//                   nowEncoderCount = 1;
//               } 

              
//               // It's time for settings !!
//               if(nowEncoderCount >= 4) {
//                   // Now set INIT() !!
//                   // Bug fixing. 150 sould close all.
//                   if(eggBadExitSTART == true) {
//                         // OFF the badExit
//                         EggExit::badExitOffOne();
//                         eggBadExitSTART = false;

//                         // Finalizing
//                         SerialUSB.println("HOME CHECK");
//                         shiftLoadCellArray();
//                         innerLoop = false;
//                   }

                  
//                   #ifdef DEBUG_INTERRUPT
//                       SerialUSB.print("HOME INIT !! Encoder[");
//                       SerialUSB.print(b_counter);
//                       SerialUSB.print("] OneStep [");
//                       SerialUSB.print(oneStepCounter);
//                       SerialUSB.print("] nowEncoderCount [");
//                       SerialUSB.print(nowEncoderCount);
//                       SerialUSB.println("]");
//                   #endif
                  
//                   oneStepMove = true;
      
//                   // Position reset
//                   oneStepCounter = 0;

//                   // nearSensor INIT
//                   nearSensorInit();

//                   if(calibration == false) pulseCount_0 = 0;

//                   // exit & bad packet receive status init(2022.01.29)
//                   // Remove.(2022.10.27)
//                   // extBadPacketRcv = false;
//                   // Serial2.println("HOME INIT"); 

//                   // Serial data waiting period setting value init (2022.01.15)
//                   serialEncoderMax = 0;

//                   // INIT the count !!
//                   nowEncoderCount = 1;
//               }
//     }
//     // sei();     //restart interrupts
   
//   }

//   // Counter plus and triggering
//   void countPlus()
//   {
      
//     //   cli(); //We pause interrupts happening before we read pin values

//       StateData  =    digitalRead(B_Encoder);

//       if(StateData == HIGH && pulseHOMECounter >= 600) {
//             pulseHOMECounter++;

//             if(pulseHOMECounter >= ledOnPosition + 600) {
//                   nowHOMEposition = true;
//                   pulseHOMECounter = 0;
//                   homeReset();
//             }
//       }
      
//       if(eggSTART){
//             StateData  =    digitalRead(B_Encoder);              //Check pin     D19 state? Data
//             if(StateData == HIGH) {
//                   b_counter++;
//                   oneStepCounter++;
//                   pulseCount_0++;  
//             }

//             if(oneStepCounter >= 150 && nowEncoderCount <= 3) {

//                   // Bug fixing. 150 sould close all.
//                   if(eggBadExitSTART == true) {
//                         // OFF the badExit
//                         EggExit::badExitOffOne();
//                         eggBadExitSTART = false;

//                         SerialUSB.println("COUNTER CHECK");
//                         // Finalizing
//                         shiftLoadCellArray();
//                         innerLoop = false;
//                   }

//                   #ifdef DEBUG_INTERRUPT
//                       SerialUSB.print("Encoder[");
//                       SerialUSB.print(b_counter);
//                       SerialUSB.print("] OneStep [");
//                       SerialUSB.print(oneStepCounter);
//                       SerialUSB.print("] nowEncoderCount [");
//                       SerialUSB.print(nowEncoderCount);
//                       SerialUSB.println("]");
//                   #endif
                  
//                   oneStepMove = true;
      
//                   // Position reset
//                   oneStepCounter = 0;

//                   // nearSensor INIT
//                   nearSensorInit();

//                   if(calibration == false) pulseCount_0 = 0;

//                   // Serial data waiting period setting value init (2022.01.15)
//                   serialEncoderMax = 0;

//                   // now status update
//                   nowEncoderCount++;

//             }
//       }
//     //   sei();     //restart interrupts
//   }

// Interrupt/Interrupt.cpp

#include "Interrupt.h"
#include "config/VariableEggBoard.hpp"
#include "config/Define.hpp"
#include "Utility/Utility.h"
#include "EggExit/EggExit.h"
#include "NearSensor/NearSensor.h"
#include "LEDControl/LEDControl.h"


void homeResetISR()
{

    int homePositionLocal = digitalRead(inputRESET);

    if (homePositionLocal == HIGH) {
        pulseHOMECounter = 600;
    }

    if (nowHOMEposition) {
        g_homeEvent = true;
    }
}


void countPlusISR()
{
    int state = digitalRead(B_Encoder);

    // 역할 분리
    if(!eggSTART) {
        if (state == HIGH && !nowHOMEposition){
            pulseHOMECounter++;
            
            if(pulseHOMECounter >= 600){
                nowHOMEposition   = true; 
                pulseHOMECounter  = 0;
                g_homeEvent       = true;  
            }
        }else if(state == LOW){
            pulseHOMECounter = 0;
        }
    }

    if (eggSTART) {
        if (state == HIGH) {
            b_counter++;
            oneStepCounter++;
            pulseCount_0++;
        }

        if (oneStepCounter >= 150 && nowEncoderCount <= 3) {
            oneStepMove      = true;   
            g_stepBoundaryEvent = true;
        }
    }
}


void handleHomeEvent()
{
    g_homeEvent = false;

    int homePositionLocal = digitalRead(inputRESET);

    if (homePositionLocal == HIGH) {
        pulseHOMECounter = 600;
    }

    if (nowHOMEposition) {
        nowHOMEposition   = false;
        pulseHOMECounter  = 0;

        // #ifdef DEBUG_INTERRUPT
        // SerialUSB.print("HOME [");
        // SerialUSB.print(b_counter);
        // SerialUSB.println("]");
        // #endif

        b_counter = 0;

        if (!eggSTART && runMode == ENCODER_MODE) {
            eggSTART        = true;
            oneStepMove     = true;
            oneStepCounter  = 0;
            nowEncoderCount = 1;

            // #ifdef DEBUG_INTERRUPT
            // SerialUSB.println("Home position. This is encoder start point. Start settled !!!!!");
            // #endif
        }

        if (nowEncoderCount >= 4) {
            if (eggBadExitSTART) {
                EggExit::badExitOffOne();
                eggBadExitSTART = false;

                SerialUSB.println("HOME CHECK");
                shiftLoadCellArray();
                innerLoop = false;
            }

            // #ifdef DEBUG_INTERRUPT
            // SerialUSB.print("HOME INIT !! Encoder[");
            // SerialUSB.print(b_counter);
            // SerialUSB.print("] OneStep [");
            // SerialUSB.print(oneStepCounter);
            // SerialUSB.print("] nowEncoderCount [");
            // SerialUSB.print(nowEncoderCount);
            // SerialUSB.println("]");
            // #endif

            oneStepMove    = true;
            oneStepCounter = 0;

            nearSensorInit();

            if (!calibration) {
                pulseCount_0 = 0;
            }
            serialEncoderMax = 0;
            nowEncoderCount = 1;
        }
    }
}

void handleStepBoundaryEvent()
{
    g_stepBoundaryEvent = false;

    if (!eggSTART) return;

    if (oneStepCounter < 150 || nowEncoderCount > 3) {
        return;
    }

    if (eggBadExitSTART) {
        EggExit::badExitOffOne();
        eggBadExitSTART = false;

        SerialUSB.println("COUNTER CHECK");
        shiftLoadCellArray();
        innerLoop = false;
    }

    #ifdef DEBUG_INTERRUPT
    SerialUSB.print("Encoder[");
    SerialUSB.print(b_counter);
    SerialUSB.print("] OneStep [");
    SerialUSB.print(oneStepCounter);
    SerialUSB.print("] nowEncoderCount [");
    SerialUSB.print(nowEncoderCount);
    SerialUSB.println("]");
    #endif

    oneStepMove    = true;
    oneStepCounter = 0;

    nearSensorInit();

    if (!calibration) {
        pulseCount_0 = 0;
    }

    serialEncoderMax = 0;

    nowEncoderCount++;
}
