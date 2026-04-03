#include "NearSensor/NearSensor.h"
#include "config/Define.hpp"
#include "config/VariableEggBoard.hpp"
#include "Utility/Utility.h"
  
  /**
   * NearSensor.cpp
   * 
   * agr.pj
   * 
   * 2025.04.22
   */

  // Specified time read near sensors / Not always. 
  void readNearSensor(){
    for (int i = 0; i < 8; i++) {
    int sensorValue = digitalRead(NEAR_SENSOR_PINS[i]);
    sensor_status[i] = (sensorValue == HIGH); // HIGH 일 경우 true LOW 일 경우 false
  }
}

void nearSensorInit() {

  // Init whole the Sensor Value false;
  for(int i=0; i < sizeof(sensor_status) ; i++) {
        sensor_status[i] = false;
  }
}

// NEAR SENSOR DATA MAKING & SENDING
void syncPulse() {

  // Egg Exist bit packet make and seding to 
  // Sync Pulse , 'P' 0x50, 1byte
  uint8_t syncPulse[]={0x7E, 0x03, 0x50, 0x00, 0x00, 0x00};
  byte    eggExistBit = 0b00000000;
  

  eggExistBit = eggBoardLoadCell[0][proximityStep-0];

  #ifdef DEBUG_EXIST
      SerialUSB.print("EGG EXIST BIT : [");
      SerialUSB.print(eggExistBit, BIN);
      SerialUSB.println("]");
  #endif

  getNowSequence();
  syncPulse[3] = programSequenceHex[1];
  syncPulse[4] = eggExistBit;

  byte checkSum = 0x00;

  for(int i=1 ; i<5 ; i++){
      checkSum += syncPulse[i];
  }
  
  checkSum = checkSum & 0xff;

  syncPulse[5] = checkSum;

  // AIP sending
  Serial2.write(syncPulse, sizeof(syncPulse));

  #ifdef DEBUG
      SerialUSB.print("SyncPulse Data:");
      for(int i=0; i < sizeof(syncPulse); i++){
          SerialUSB.print(syncPulse[i], HEX);
          SerialUSB.print(" ");
      }
      SerialUSB.println("");
  #endif

} // End of Sync Pulse


// NEAR SENSOR DATA MAKING to ARRAY
void eggExistSetting() {

  // Egg Exist bit packet make and seding to 
  // Sync Pulse , 'P' 0x50, 1byte
  uint8_t syncPulse[]={0x7E, 0x03, 0x50, 0x00, 0x00, 0x00};
  byte eggExistBit = 0b00000000;

  // Init nearSensor values.
  for(int i=0; i<sizeof(sensor_status); i++){
    if(sensor_status[i]) bitWrite(eggExistBit, i, 1);
  }
  
  //eggBoardLoadCell[0][0] 무조건 세팅
  //배열에 세팅 해 줌.
  eggBoardLoadCell[0][0] = eggExistBit;

  #ifdef DEBUG_EXIST
      SerialUSB.print("EGG EXIST BIT : [");
      SerialUSB.print(eggExistBit, BIN);
      SerialUSB.println("]");
  #endif
} // End of Sync Pulse



// NEAR SENSOR DATA MAKING & SENDING
void syncPulseTimer() {

  // Egg Exist bit packet make and seding to 
  // Sync Pulse , 'P' 0x50, 1byte
  uint8_t syncPulse[]={0x7E, 0x03, 0x50, 0x00, 0x00, 0x00};
  byte eggExistBit = 0b00111111;

  // 고정된 값으로 전송 (Timer Mode 임)


  SerialUSB.print("SENSOR ON and data SET : [");
  SerialUSB.print(eggExistBit, BIN);
  SerialUSB.println("]");

  getNowSequence();
  
  syncPulse[3] = programSequenceHex[1];
  syncPulse[4] = eggExistBit;

  byte checkSum = 0x00;

  for(int i=1 ; i<5 ; i++){
      checkSum += syncPulse[i];
  }
  
  checkSum = checkSum & 0xff;

  syncPulse[5] = checkSum;


  // AIP sending
  Serial2.write(syncPulse, sizeof(syncPulse));
  
} // End of Sync Pulse

