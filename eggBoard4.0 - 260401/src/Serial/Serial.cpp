#include "Serial.h"
#include "config/VariableEggBoard.hpp"
#include "FlashROM/FlashROM.h"
#include "LEDControl/LEDControl.h"
#include "Utility/Utility.h"
#include "EggExit/EggExit.h"
#include "defines/struct_device_cmd.hpp"
#include "CameraControl/CameraControl.h"

static DEVICE_CONTROL_CMD commandType;

bool serialProcessing() {
    
    byte inByte = Serial2.read();
    char tmpVar = '\0'; 
    int getIndex = 0;
    unsigned int setValue = 0;

    serialEncoderMax = oneStepCounter + serialEndocerStep;

    #ifdef DEBUG_SERIAL
    SerialUSB.println(inByte, HEX);
    #endif

    if (inByte == 126) {
        while (!Serial2.available()) {
            if (oneStepCounter >= serialEncoderMax) break;
        }
        inByte = Serial2.read();

        #ifdef DEBUG_SERIAL
        SerialUSB.println(inByte, HEX);
        #endif

        bufSize = int(inByte);
        int checksum = bufSize;

        for (int i = 0; i < bufSize;) {
            if (Serial2.available()) {
                toCheck[i] = Serial2.read();
                checksum += toCheck[i];
                i++;
                #ifdef DEBUG_SERIAL
                SerialUSB.println((uint8_t)toCheck[i], HEX);
                #endif
            } else if (oneStepCounter >= serialEncoderMax) i = bufSize;
        }

        checksum &= 0xff;

        while (!Serial2.available()) {
            if (oneStepCounter >= serialEncoderMax) break;
        }
        byte orgchecksum = Serial2.read();

        #ifdef DEBUG_SERIAL
        SerialUSB.print("CHECK SUM : [");
        SerialUSB.print(checksum, HEX);
        SerialUSB.print(" ");
        SerialUSB.print(orgchecksum, HEX);
        SerialUSB.println("]");
        #endif

        if (checksum == orgchecksum) {

            SerialUSB.println("CHECKSUM OK");

            char ourCommand = toCheck[0];
            SerialUSB.println(ourCommand);

            switch(ourCommand){

            case 'S':{
                SerialUSB.println("S");

                getIndex = int(toCheck[1]);

                SerialUSB.print("INDEX : ");
                SerialUSB.println(getIndex);


                if(getIndex == 0x00) {           //LED POSITION
                    SerialUSB.print("LED POSITION [");

                    // Value 변환 (byte --> int)
                    
                    ledOnPosition = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];

                    SerialUSB.print(ledOnPosition);
                    SerialUSB.println("]");

                    Save_FlashData();
                    saveResponse();

                  
                } else if(getIndex == 0x01) {    // LED BRIGHTNESS
                    SerialUSB.print("LED BRIGHTNESS [");

                    ledBrightness = ((uint16_t)(uint8_t)toCheck[2] << 8) | (uint16_t)(uint8_t)toCheck[3];
                    for(int i = 0; i < ledCount; i++) {
                        ledEnvArrayInt[0][i] = ledBrightness;
                    }                    
                    SerialUSB.print(ledBrightness);
                    SerialUSB.println("]");   

                    intArrayToByteArray();

                    Save_FlashData();
                    saveResponse();
                  
                } else if(getIndex == 0x02) {    // LED DURATION
                    SerialUSB.print("LED DURATION [");

                    ledDuration = ((uint16_t)(uint8_t)toCheck[2] << 8) | (uint16_t)(uint8_t)toCheck[3];
                    for (int i = 0; i < ledCount; i++) {
                        ledEnvArrayInt[1][i] = ledDuration;   // ★ duration int 배열도 업데이트
                    }
                    
                    SerialUSB.print(ledDuration);
                    SerialUSB.println("]");

                    intArrayToByteArray();

                    saveResponse();

                    Save_FlashData();
                 
                } else if(getIndex == 0x03) {    // TRIGGER DELAY
                    SerialUSB.print("TRIGGER DELAY [");

                    triggerDelay = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(triggerDelay);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                  
                } else if(getIndex == 0x04) {    // TRIGGER DURATION
                    SerialUSB.print("TRIGGER DURATION [");

                    triggerDuration = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(triggerDuration);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                    
                } else if(getIndex == 0x05) {    // OPTION BITS
                    SerialUSB.print("OPTION BITS [");

                    optionBits = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(optionBits);
                    SerialUSB.println("]");

                    // LED OPTION SET
                    setLEDarray(optionBits);

                    saveResponse();

                    Save_FlashData();
                  
                } else if(getIndex == 0x06) {    // PROXIMITY STEP
                    SerialUSB.print("PROXIMITY STEP [");

                    proximityStep = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(proximityStep);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                  
                } else if(getIndex == 0x07) {    // LOADCEL STEP
                    SerialUSB.print("LOADCEL STEP [");
                    loadCellStep = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(loadCellStep);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                  
                } else if(getIndex == 0x08) {    // OUTPUT POSITION
                    SerialUSB.print("OUTPUT POSITION [");

                    outputPosition = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(outputPosition);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                  
                } else if(getIndex == 0x09) {    // Crack Output Set
                    SerialUSB.print("OUTPUT DURATION [");

                    outputDuration = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(outputDuration);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                    
                  
                } else if(getIndex == 0x0A) {    // Bad Output Set
                    SerialUSB.print("BAD OUTPUT STEP [");

                    badOutputStep = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(badOutputStep);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                    
                  
                } else if(getIndex == 0x0B) {    // LED individual Set
                    SerialUSB.print("BAD OUTPUT POSITION [");

                    badOutputPosition = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(badOutputPosition);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                    
                  
                } else if(getIndex == 0x0C) {    // LED group Set

                    SerialUSB.print("BAD OUTPUT DURATION [");

                    badOutputDuration = (uint8_t)toCheck[2] * 256 + (uint8_t)toCheck[3];
                    
                    SerialUSB.print(badOutputDuration);
                    SerialUSB.println("]");

                    saveResponse();

                    Save_FlashData();
                    
                  
                } else if(getIndex == 0xFF) {  // WHOLE ITEM SETTINGS
                    SerialUSB.print("WHOLE ITEM SETTINGS [");

                    // JUST RESPONSE. 
                    
                    saveResponse();
                } else {
                    SerialUSB.println("IVALID ARGUMENTS");
                    invalidArgument();
                }

                break;
            
          }
              
              
          case 'W':{
              SerialUSB.println("W");
              SerialUSB.println("W , SAVE ENV VALUES to FlshRom");
              //WHOLE DATA SAVE

              Save_FlashData();
            
              uint8_t resultString[5] = {0x7E, 0x02, 0x77, 0x00, 0x00};
              resultString[4] = (uint8_t)(resultString[1] + resultString[2] + resultString[3]);
              Serial2.write(resultString, sizeof(resultString));

              break;
          }

          case 'V':{
              SerialUSB.println("Version Req");  //  Version Req

              SerialUSB.println("7E 05 76 48 42 01 01 07");

              uint8_t resultString8[8] = {0x7E,0x05,0x76,0x48,0x42,0x01,0x01,0x07};

              Serial2.write(resultString8, sizeof(resultString8));
            //   Serial2.write('\n');
              
              break;
          }
              
          case 'M':{
              SerialUSB.println("M");  // Get Run Mode Req, 'm' response
              SerialUSB.println("Get Run Mode Req");  // Get Run Mode Req, 'm' response

              byte resRunMode = (uint8_t)runMode;

              SerialUSB.println(resRunMode, HEX);
              byte checksum = 0x02 + 0x6D + resRunMode;

              checksum = checksum & 0xff;

              uint8_t resultString[5] = {0x7E, 0x02, 0x6D, resRunMode, checksum};

              for(int i=0; i< sizeof(resultString); i++ ){
                  SerialUSB.print("[");
                  SerialUSB.print(resultString[i], HEX);
                  SerialUSB.print("]");
              }
              SerialUSB.println("");


              Serial2.write(resultString, sizeof(resultString));

              break;
          }
              
          case 'N':{ // toCheck에 아스키값으로 들어오는지 ,순수 바이너리 값으로 들어오는지 확인
              SerialUSB.println("N");            // Set Run mode , no response
              SerialUSB.println("and");            // Set Run mode , no response

              runMode = (uint8_t)toCheck[1];

              SerialUSB.print("Run Mode [");
              SerialUSB.print(runMode);
              SerialUSB.println("]");

              if(runMode == ENCODER_MODE){
                    // Encoder mode
                    eggSTART =    false;
                    timerSTART =  false;
                    manualSTART = false;

                    eggStartPreTime = 0;

                    // Previous led all off !! (2022.03.17). bug reported. Any mode at previous whole the led off now !!
                    ledOff();
                    // 추가
                    ledSTART        = false;
                    ledEachStart    = false;
                    ledOnDelayCount = 0;
                    ledOnOffLoopCNT = 0;
                    nowLedOffCount  = 0;

                    for(int i = 0; i < ledCount; i++) {
                        ledPreTimeArray[i] = millis();
                    }

                    SerialUSB.println("MODE CHANGED -----> ENCODER MODE");
                
              } else if(runMode == TIMER_MODE){
                    // TIMER mode, Encoder mode stop
                    eggSTART =    false;
                    timerSTART =  true;
                    manualSTART = false;

                    // Previous led all off !! (2022.03.17). bug reported. Any mode at previous whole the led off now !!
                    ledOff();
                    
                    SerialUSB.println("MODE CHANGED -----> TIMER MODE");
                
              } else if(runMode == MANUAL_MODE){
                    // MANUAL mode, Encoder mode stop
                    eggSTART =    false;
                    timerSTART =  false;
                    manualSTART = true;

                    // Previous led all off !! (2022.03.17). bug reported. Any mode at previous whole the led off now !!
                    ledOff();
                    
                    SerialUSB.println("MODE CHANGED -----> MANUAL MODE");
              }

              Save_FlashData();
              
              break;
          }
              
          case 'T':{
              SerialUSB.println("TRIGGER Enable Bits");

              eggExistBit_manual = (uint8_t)toCheck[2];
      
              SerialUSB.print("MANUAL EGG EXIST BIT : [");
              SerialUSB.print(eggExistBit_manual, BIN);
              SerialUSB.println("]");

              if (runMode == MANUAL_MODE) {
                uint8_t syncPulse[6] = {0x7E, 0x03, 0x50, 0x00, 0x00, 0x00};
                
                getNowSequence();
                syncPulse[3] = (uint8_t)programSequenceHex[1]; // data_seq
                syncPulse[4] = eggExistBit_manual; 

                uint16_t sum = 0;
                for (int i = 1; i <= 1 + syncPulse[1]; ++i) sum += syncPulse[i];  // [1]..[4]
                syncPulse[5] = (uint8_t)sum;

                for (int i=0;i<6;i++){ SerialUSB.print(syncPulse[i], HEX); SerialUSB.print(' '); }
                SerialUSB.println();

                Serial2.write(syncPulse, sizeof(syncPulse));
                triggerSTART = true;
            } else {
                SerialUSB.println("Not Manual Mode");
            }

            // ACK
            uint8_t resp[5] = {0x7E, 0x02, 0x74, 0x00, 0x00};
            resp[4] = (uint8_t)(resp[1] + resp[2] + resp[3]);
            Serial2.write(resp, sizeof(resp));
            break;
            }
              
          case 'E':{
              SerialUSB.println("Manual Crack Status Bits");
              
              crackStatusBit_manual = (uint8_t)toCheck[1];
              badStatusBit_manual =   (uint8_t)toCheck[2];


              SerialUSB.print("MANUAL Crack Status Bits : [");
              SerialUSB.print(crackStatusBit_manual, BIN);
              SerialUSB.print("][");
              SerialUSB.print(badStatusBit_manual, BIN);
              SerialUSB.println("]");
            
              // Packet Make & Send

              if(runMode == MANUAL_MODE){
                
                    EggExit::crackExitOn(crackStatusBit_manual);
                    EggExit::badExitOn(badStatusBit_manual);

                    boolean loopNow = false;
                    boolean loopNowBad = false;
                    SerialUSB.println("While Start");

                    int crackExitCount = 0;
                    int badExitCount = 0;
                    

                    while(loopNow == false || loopNowBad == false){
                         unsigned long currentTimeloop = millis();
                        
                          if(currentTimeloop - eggExitDurationPreTime >= outputDuration) { 
                                crackExitCount++;

                                if(crackExitCount == 2) {
                                    EggExit::crackExitOff(crackStatusBit_manual);
                                    SerialUSB.print("CRACK OFF ~DURING [");
                                    SerialUSB.print(currentTimeloop - eggExitDurationPreTime);
                                    SerialUSB.println("]");
                                    loopNow = true;
                                }
                                eggExitDurationPreTime = currentTimeloop;  
                          }

                          if(currentTimeloop - eggBadExitDurationPreTime >= badOutputDuration) { 
                                badExitCount++;

                                if(badExitCount == 2) {
                                  
                                    EggExit::badExitOff(crackStatusBit_manual);
                                    SerialUSB.print("BAD OFF ~");
                                    SerialUSB.print(currentTimeloop - eggBadExitDurationPreTime);
                                    SerialUSB.println("]");
                                    loopNowBad = true;
                                }
                                eggBadExitDurationPreTime = currentTimeloop;  
                          }
                          
                    }

                    SerialUSB.println("While End");

                
              } else {
                SerialUSB.println("Not Manual Mode");
              }
              // Response Make and Sending !! 'e', 65

              uint8_t resultString[5] = {0x7E, 0x02, 0x65, 0x00, 0x67};

              for(int i=0; i< sizeof(resultString); i++ ){
                  SerialUSB.print("[");
                  SerialUSB.print(resultString[i], HEX);
                  SerialUSB.print("]");
              }
              SerialUSB.println("");

              Serial2.write(resultString, sizeof(resultString));
              
              break;
          }
              
          case 'I':{
              SerialUSB.println("Manual Timer Period Setting");
            
              manualTimerPeriod = ((uint16_t)(uint8_t)toCheck[1] << 8) + (uint16_t)(uint8_t)toCheck[2];
            
              SerialUSB.print("manualTimerPeriod [");
              SerialUSB.print(manualTimerPeriod);
              SerialUSB.println("]");
                SerialUSB.print("toCheck[0] Data Check");
                SerialUSB.print(toCheck[0], HEX);
                SerialUSB.print("toCheck[1] Data Check");
                SerialUSB.print(toCheck[1], HEX);
                SerialUSB.print("toCheck[2] Data Check");
                SerialUSB.print(toCheck[2], HEX);
                SerialUSB.print("toCheck[3] Data Check");
                SerialUSB.print(toCheck[3], HEX);
                SerialUSB.print("toCheck[4] Data Check");
                SerialUSB.print(toCheck[4], HEX);
                SerialUSB.print("toCheck[5] Data Check");
                SerialUSB.print(toCheck[5], HEX);
                SerialUSB.print("toCheck[6] Data Check");
                SerialUSB.print(toCheck[6], HEX);

              uint8_t resultString[5] = {0x7E, 0x02, 0x69, 0x00, 0x76};
              resultString[4] = (uint8_t)(resultString[1] + resultString[2] + resultString[3]);
              for(int i=0; i< sizeof(resultString); i++ ){
                  SerialUSB.print("[");
                  SerialUSB.print(resultString[i], HEX);
                  SerialUSB.print("]");
              }
              SerialUSB.println("");

              Serial2.write(resultString, sizeof(resultString));

              break;
          }
          case 'Q':{
              SerialUSB.println("Q");
              SerialUSB.println("Reboot Packet Received.. REBOOT ...");

              uint8_t resultString[5] = {0x7E, 0x02, 0x71, 0x00, 0x73};

              for(int i=0; i< sizeof(resultString); i++ ){
                  SerialUSB.print("[");
                  SerialUSB.print(resultString[i], HEX);
                  SerialUSB.print("]");
              }
              SerialUSB.println("");

              
              // asm volatile ("jmp 0");
              reboot();
              
              Serial2.write(resultString, sizeof(resultString));

              break;
          }
          case 'L':{
              SerialUSB.println("Manual LED OnTimer Setting");

              uint8_t ledStatus01 = (uint8_t)toCheck[1];
              uint8_t ledStatus02 = (uint8_t)toCheck[2];
              uint8_t ledStatus03 = (uint8_t)toCheck[3];
              uint8_t ledStatus04 = (uint8_t)toCheck[4];
            //   uint8_t ledStatus05 = (uint8_t)toCheck[5];
              

              SerialUSB.print("[");
              SerialUSB.print(ledStatus01, BIN);
              SerialUSB.print("][");
              SerialUSB.print(ledStatus02, BIN);
              SerialUSB.print("][");
              SerialUSB.print(ledStatus03, BIN);
              SerialUSB.print("][");
              SerialUSB.print(ledStatus04, BIN);
              SerialUSB.print("][");
            //   SerialUSB.print(ledStatus05, BIN);
            //   SerialUSB.print("]");

              // REPLY TO AIP
              uint8_t resultString[5] = {0x7E, 0x02, 0x6C, 0x00, 0x6E};
              #ifdef DEBUG
                  for(int i=0; i< sizeof(resultString); i++ ){
                      SerialUSB.print("[");
                      SerialUSB.print(resultString[i], HEX);
                      SerialUSB.print("]");
                  }
                  SerialUSB.println("");
              #endif

              Serial2.write(resultString, sizeof(resultString));
              

              // LED ON

              if(runMode == MANUAL_MODE){

                    SerialUSB.println("LED ON ~~ ");

                    ledManualON(ledStatus01, ledStatus02, ledStatus03, ledStatus04 , 0);

                    boolean loopNow = false;
                    SerialUSB.println("While Start");

                    int crackExitCount = 0;
                    

                    while(loopNow == false){
                          unsigned long currentTimeloop = millis();

                          //timer 처리 (Duration 이후 OFF)
                          if(currentTimeloop - ledPreTime >= ledDuration) {
                                ledOnCount++;
        
                                if(ledOnCount == 2){
                                      
                                      SerialUSB.print("LED OFF ~ count:[");
                                      SerialUSB.print(ledOnCount);
                                      SerialUSB.print("][");
                                      SerialUSB.print(currentTimeloop - ledPreTime);
                                      SerialUSB.print("][");
                                      SerialUSB.print(ledDuration);
                                      SerialUSB.println("]");
                                  
                                      ledManualOff(ledStatus01, ledStatus02, ledStatus03 , ledStatus04,0);
                                      
                                      
                                      ledOnCount = 0;
                                      loopNow = true;
                                }
                                ledPreTime = currentTimeloop;
                        }
                      
                    }    

              } else {
                SerialUSB.println("Not Manual Mode");
              } 

              break;
          }

          case 'G':{
              SerialUSB.println("COMMAND G, REQUEST WHOLE SETTING VALUES");

              tmpVar = toCheck[1];
              

              getIndex = int((uint8_t)toCheck[1]);
              
              
              SerialUSB.println("");
              
              SerialUSB.print("INDEX : ");
              SerialUSB.println(getIndex, HEX);
              
              // Compare with HEX value
              if(getIndex == 0xFF) {
                  SerialUSB.print("ALL VALUE REQUESTED ");
                  SerialUSB.println(getIndex);
              }

              // Response making

              uint8_t responseObj[31]={0x7E,0x1C,0x67,0xFF};            // Sending Array
              byte b[2];
              uint8_t checkSumCal =  0x1C;

              byte tmpValue = checkSumCal & 0xff;
              

              checkSumCal = tmpValue;

              
              checkSumCal += 0x67;
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              
              checkSumCal += 0xFF;
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;


              integerToBytes(ledOnPosition, b);
              responseObj[4] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              
              responseObj[5] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              

              integerToBytes(ledBrightness, b);
              responseObj[6] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[7] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              integerToBytes(ledDuration, b);
              responseObj[8] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[9] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              

              integerToBytes(triggerDelay, b);
              responseObj[10] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[11] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              

              integerToBytes(triggerDuration, b);
              responseObj[12] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[13] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;


              

              integerToBytes(optionBits, b);
              responseObj[14] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[15] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;


              

              integerToBytes(proximityStep, b);
              responseObj[16] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[17] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;


              

              integerToBytes(loadCellStep, b);
              responseObj[18] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[19] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;


              

              integerToBytes(outputPosition, b);
              responseObj[20] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[21] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              

              integerToBytes(outputDuration, b);
              responseObj[22] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[23] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              

              integerToBytes(badOutputStep, b);
              responseObj[24] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[25] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              

              integerToBytes(badOutputPosition, b);
              responseObj[26] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;
              
              responseObj[27] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              

              integerToBytes(badOutputDuration, b);
              responseObj[28] = b[0];
              checkSumCal += b[0];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              responseObj[29] = b[1];
              checkSumCal += b[1];
              tmpValue = checkSumCal & 0xff;
              checkSumCal = tmpValue;

              responseObj[30] = checkSumCal;

              for(int i=0; i< sizeof(responseObj); i++ ){
                  SerialUSB.print("[");
                  SerialUSB.print(responseObj[i], HEX);
                  SerialUSB.print("]");
              }
              SerialUSB.println("");

              Serial2.write(responseObj, sizeof(responseObj));

              break;
          }


          case 'R':{
              #ifdef DEBUG
                  SerialUSB.print("Classification Result [Crack][BAD] buffsize[");  // Crack Status Bits (1 byte), 1,2,3,4,5,6,7(x), 8(x)
                  SerialUSB.print(bufSize);
                  SerialUSB.println("]");
              #endif
              
              // CRACK ==> eggBoardLoadCell[1][proximityStep+3], BAD ==> eggBoardLoadCell[1][proximityStep+3] 배열에 세팅

              if(bufSize == 3){
                    // V1.x packet. Crack 만 지니고 있음. ############### V1.x 
                    setValue = (byte)toCheck[2];

                    // Array Settings.
                    //eggBoardLoadCell[0][proximityStep+3] = setValue;   // BAD 와 값이 같지 않음. 
                    
                    eggBoardLoadCell[0][proximityStep+4] = setValue;

                    SerialUSB.print("[CRACK ONLY] EGG CRACK BIT [");
                    SerialUSB.print(setValue, BIN);
                    SerialUSB.println("]");

                    // Packet received flag settings.
                    extPackeRcv = true;
                    
              } else if(bufSize == 4){
                    // V2.x packet. Crack, Bad 둘다 가지고 있음. ############### V2.x  
                    setValue = (byte)toCheck[2];

                    // Array Settings.
                    eggBoardLoadCell[0][proximityStep+4] = setValue;

                    extPackeRcv = true;

                    
                    #ifdef DEBUG_EXIT
                        // Values check and set to Array
                        for(int i=0; i<8; i++){
                            int bitValue = 0;
                            bitValue = bitRead(setValue, i);
                            SerialUSB.print("[");
                            SerialUSB.print(bitValue);
                            SerialUSB.print("]");
                        }
                        SerialUSB.println("");
                    #endif
                    

                    // SAVE ARRAY CRACK STAUS & RESPONSE
                    // Checksum analyzing !!!!

                    // V2.3 applied version should be bad status
                    byte bad_setValue = (byte)toCheck[3];

                    eggBoardLoadCell[1][proximityStep+4] = bad_setValue;

                    badPackeRcv = true;
                    extPackeRcv = true;

                    #ifdef DEBUG
                        SerialUSB.print("EGG CRACK [");
                        SerialUSB.print(setValue, BIN);
                        SerialUSB.print("] BAD [");
                        SerialUSB.print(bad_setValue, BIN);
                        SerialUSB.println("]");
                    #endif 
              }

              // Packet received status change (2022.01.29)
              extBadPacketRcv = true;

              
              byte tmpValue =(uint8_t)((0x02 + 0x72 + 0x00) & 0xFF);
              
              uint8_t resultString[5] = {0x7E, 0x02, 0x72, 0x00, tmpValue};

              #ifdef DEBUG_SERIAL
                  SerialUSB.print("Response Packet :");

                  for(int i=0; i< sizeof(resultString); i++ ){
                      SerialUSB.print("[");
                      SerialUSB.print(resultString[i], HEX);
                      SerialUSB.print("]");
                  }
                  SerialUSB.println("");

                  SerialUSB.print("================packet receive extBadPacketRcv ==> ");
                  SerialUSB.println(extBadPacketRcv);
                  
              #endif

              Serial2.write(resultString, sizeof(resultString));

              break;
              }
              case 'A':{
              SerialUSB.println("GET LED DATA REQUEST");  // Get LED DATA REQ, 'a' response
                
              // Make LED Status data  
              // ledEnvArray
              // 49 field
              uint8_t tmpVal = 0;
              uint8_t resultString[124] = {0x7E, 0x79, 0x61};
              SerialUSB.println("resultString initialized");
              SerialUSB.print(sizeof(resultString));
              uint8_t checkSumCal =  0x79;

              SerialUSB.println("checkSumCal initialized");
              SerialUSB.print(checkSumCal, HEX);
              checkSumCal += 0x61;
              

              for(int i=3; i < 63 ; i++) {
                    resultString[i] = ledEnvArray[0][i-3];
                    checkSumCal +=    ledEnvArray[0][i-3];
                    tmpVal =          checkSumCal & 0xff;
                    checkSumCal =     tmpVal;
              }

                SerialUSB.println("First loop done");

              for(int i=63; i < 123 ; i++) {
                    resultString[i] = ledEnvArray[1][i-63];
                    checkSumCal +=    ledEnvArray[1][i-63];
                    tmpVal =          checkSumCal & 0xff;
                    checkSumCal =     tmpVal;
              }
              SerialUSB.println("Second loop done");
              
              resultString[123] = checkSumCal;

              Serial2.write(resultString, sizeof(resultString));
              SerialUSB.println("resultString size:");
              SerialUSB.println(sizeof(resultString));

              for(int i=0; i< sizeof(resultString); i++ ){
                      SerialUSB.print(resultString[i], HEX);
                      SerialUSB.print(" ");
              }
              
              break;
          } case 'B':{
              SerialUSB.println("SET LED DATA REQUEST");  // Set LED DATA REQ, 'b' response

            //   for(int i=1; i< 63; i++){
            //       ledEnvArray[0][i-1] = (byte)toCheck[i];
            //   }
            //   for(int i=63; i< 124; i++){
            //       ledEnvArray[1][i-62] = (byte)toCheck[i];
            //   }
                for (int i = 0; i < 60; ++i) {
                ledEnvArray[0][i] = (uint8_t)toCheck[1 + i];
                }

                    for (int i = 0; i < 60; ++i) {
                ledEnvArray[1][i] = (uint8_t)toCheck[1 + 60 + i];  // 61..120
                }

              // Make LED Status data  
              // ledEnvArray
              // 49 field
              uint8_t tmpVal = 0;
              uint8_t resultString[124] = {0x7E, 0x79, 0x62};
              //ox60 : 97
              uint8_t checkSumCal =  0x79;
              checkSumCal += 0x62;
              

              for(int i=3; i < 63 ; i++) {
                    resultString[i] = ledEnvArray[0][i-3];
                    checkSumCal +=    ledEnvArray[0][i-3];
                    tmpVal =          checkSumCal & 0xff;
                    checkSumCal =     tmpVal;
              }

              for(int i=63; i < 123 ; i++) {
                    resultString[i] = ledEnvArray[1][i-63];
                    checkSumCal +=    ledEnvArray[1][i-63];
                    tmpVal =          checkSumCal & 0xff;
                    checkSumCal =     tmpVal;
              }

              resultString[123] = checkSumCal;
              
              for(int i=0; i< sizeof(resultString); i++ ){
                  SerialUSB.print("[");
                  SerialUSB.print(resultString[i], HEX);
                  SerialUSB.print("]");
              }
              
              Serial2.write(resultString, sizeof(resultString));

              // WHOLE THE LED DATA SAVE to FlshRom !!!

              bytesToIntegerArray();

              // FlshRom WRITE

              saveLedArray_FlashData();

              break;
          }
          case 'C': {
                int camIdx = toCheck[1];
                int modeVal = toCheck[2];
                int stateVal = toCheck[3];

                if (camIdx >= 0 && camIdx < 4) {
                    commandType.cameraModes[camIdx] = static_cast<CameraMode>(modeVal);
                    commandType.cameraStatus[camIdx] = (stateVal == 1);

                    SerialUSB.print("CAMERA[");
                    SerialUSB.print(camIdx);
                    SerialUSB.print("] MODE=");
                    SerialUSB.print(modeVal);
                    SerialUSB.print(" STATUS=");
                    SerialUSB.println(stateVal ? "ON" : "OFF");

                    TriggerController::setCameraState(camIdx, stateVal == 1);
                }
                break;
            }

            default:
                SerialUSB.println("WRONG COMMAND");  
                SerialUSB.println("IVALID ARGUMENTS");
                invalidArgument();
            }
            } else {
            SerialUSB.println("CHECKSUM FAIL");
            invalidArgument();
        }
    }
        return true;
}

void saveResponse() {
    uint8_t resultString[5] = {0x7E, 0x02, 0x73, 0x00, 0x75};

    for (int i = 0; i < sizeof(resultString); i++) {
        SerialUSB.print("[");
        SerialUSB.print(resultString[i], HEX);
        SerialUSB.print("]");
    }
    SerialUSB.println("");
    SerialUSB.print(eggBoard.ledBrightness);
    Serial2.write(resultString, sizeof(resultString));
}

void invalidArgument() {
    uint8_t resultString[5] = {0x7E, 0x02, 0x73, 0x02, 0x77};

    for (int i = 0; i < sizeof(resultString); i++) {
        SerialUSB.print("[");
        SerialUSB.print(resultString[i], HEX);
        SerialUSB.print("]");
    }
    SerialUSB.println("");


    Serial2.write(resultString, sizeof(resultString));
}