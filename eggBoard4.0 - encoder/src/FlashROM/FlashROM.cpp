#include "FlashROM.h"
#include "config/VariableEggBoard.hpp"
#include "Utility/Utility.h"

DueFlashStorage dueFlashStorage;
#define ALIGN4(x) (((x) + 3u) & ~3u)

static const int FLASH_ADDR = 0;

bool Read_FlashData() {
    // memcpy(&eggBoard, dueFlashStorage.readAddress(0), sizeof(eggBoard));

    // SerialUSB.print("runMode: ");
    // SerialUSB.println(eggBoard.runMode);

    // runMode =             eggBoard.runMode;
    // ledOnPosition =       eggBoard.ledOnPosition;
    // ledBrightness =       eggBoard.ledBrightness;
    // ledDuration =         eggBoard.ledDuration;
    // triggerDelay =        eggBoard.triggerDelay;
    // triggerDuration =     eggBoard.triggerDuration;
    // optionBits =          eggBoard.optionBits;
    // proximityStep =       eggBoard.proximityStep;
    // loadCellStep =        eggBoard.loadCellStep;
    // outputPosition =      eggBoard.outputPosition;
    // outputDuration =      eggBoard.outputDuration;
    // badOutputStep =       eggBoard.badOutputStep;
    // badOutputPosition =   eggBoard.badOutputPosition;
    // badOutputDuration =   eggBoard.badOutputDuration;
    // homePlus =            eggBoard.homePlus;

    // SerialUSB.println("=== Read Flash Data ===");
    // SerialUSB.print("runMode: "); SerialUSB.println(runMode);
    // SerialUSB.print("ledOnPosition: "); SerialUSB.println(ledOnPosition);
    // SerialUSB.print("ledBrightness: "); SerialUSB.println(ledBrightness);
    // SerialUSB.print("triggerDelay: "); SerialUSB.println(triggerDelay);
    // SerialUSB.print("homePlus: "); SerialUSB.println(homePlus);

    // // return (ledOnPosition != 0);
    // return (runMode >= 0 && ledBrightness > 0 && ledDuration > 0);
    uint32_t eeAddress = FLASH_ADDR;

    // eggBoard 로드
    memcpy(&eggBoard, dueFlashStorage.readAddress(eeAddress), sizeof(eggBoard));
    eeAddress = ALIGN4(eeAddress + sizeof(eggBoard));

    // ledEnvArrayInt 로드
    memcpy(ledEnvArrayInt, dueFlashStorage.readAddress(eeAddress), sizeof(ledEnvArrayInt));

    // eggBoard
    runMode         = eggBoard.runMode;
    ledOnPosition   = eggBoard.ledOnPosition;
    ledBrightness   = eggBoard.ledBrightness;
    ledDuration     = eggBoard.ledDuration;
    triggerDelay    = eggBoard.triggerDelay;
    triggerDuration = eggBoard.triggerDuration;
    optionBits      = eggBoard.optionBits;
    proximityStep   = eggBoard.proximityStep;
    loadCellStep    = eggBoard.loadCellStep;
    outputPosition  = eggBoard.outputPosition;
    outputDuration  = eggBoard.outputDuration;
    badOutputStep   = eggBoard.badOutputStep;
    badOutputPosition = eggBoard.badOutputPosition;
    badOutputDuration = eggBoard.badOutputDuration;
    homePlus        = eggBoard.homePlus;

    // 유효성 체크 
    bool valid =
        (runMode >= 0) &&
        (ledBrightness > 0) &&
        (ledDuration  > 0);

    return valid;
}

void Save_FlashData() {
    eggBoard.runMode =            runMode;
    eggBoard.ledOnPosition =      ledOnPosition;
    eggBoard.ledBrightness =      ledBrightness;
    eggBoard.ledDuration =        ledDuration;  
    eggBoard.triggerDelay =       triggerDelay;
    eggBoard.triggerDuration =    triggerDuration;
    eggBoard.optionBits =         optionBits;
    eggBoard.proximityStep =      proximityStep;
    eggBoard.loadCellStep =       loadCellStep;
    eggBoard.outputPosition =     outputPosition;
    eggBoard.outputDuration =     outputDuration;
    eggBoard.badOutputStep =      badOutputStep;
    eggBoard.badOutputPosition =  badOutputPosition;
    eggBoard.badOutputDuration =  badOutputDuration;
    eggBoard.homePlus =           homePlus;

    // intArrayToByteArray();
    uint32_t eeAddressTemp = FLASH_ADDR;
    dueFlashStorage.write(eeAddressTemp, (byte*)&eggBoard, sizeof(eggBoard));
    eeAddressTemp = ALIGN4(eeAddressTemp + sizeof(eggBoard));
    dueFlashStorage.write(eeAddressTemp, (byte*)ledEnvArrayInt, sizeof(ledEnvArrayInt));
}

void saveLedArray_FlashData() {
    uint32_t eeAddressTemp = FLASH_ADDR;
    // dueFlashStorage.write(eeAddressTemp, (byte*)&eggBoard, sizeof(eggBoard));    
    eeAddressTemp = ALIGN4(eeAddressTemp + sizeof(eggBoard));
    dueFlashStorage.write(eeAddressTemp, (byte*)ledEnvArrayInt, sizeof(ledEnvArrayInt));
}

void getLedArray_FlashData() {
    // uint32_t eeAddress = FLASH_ADDR;

    // memcpy(&eggBoard, dueFlashStorage.readAddress(eeAddress), sizeof(eggBoard));

    // // 2) ledEnvArrayInt 로드: eggBoard 뒤 4바이트 정렬 오프셋
    // eeAddress = ALIGN4(eeAddress + sizeof(eggBoard));
    // memcpy(ledEnvArrayInt, dueFlashStorage.readAddress(eeAddress), sizeof(ledEnvArrayInt));
    
    // SerialUSB.println("ledEnvArrayInt detail");
    // for (int i = 0; i < 30; i++) {
    //     SerialUSB.print("[ brightness : ");
    //     SerialUSB.print(ledEnvArrayInt[0][i]);
    //     SerialUSB.print(", duration : ");
    //     SerialUSB.print(ledEnvArrayInt[1][i]);
    //     SerialUSB.print("]");
    // }
    // SerialUSB.println();
}

void print_FlashData() {
    SerialUSB.println("ENV VALUES ..... ");
    SerialUSB.print("runMode: ");           SerialUSB.println(runMode);
    SerialUSB.print("ledOnPosition: ");     SerialUSB.println(ledOnPosition);
    SerialUSB.print("ledBrightness: ");     SerialUSB.println(ledBrightness);
    SerialUSB.print("ledDuration: ");       SerialUSB.println(ledDuration);
    SerialUSB.print("triggerDelay: ");      SerialUSB.println(triggerDelay);
    SerialUSB.print("triggerDuration: ");   SerialUSB.println(triggerDuration);
    SerialUSB.print("optionBits: ");        SerialUSB.println(optionBits);
    SerialUSB.print("proximityStep: ");     SerialUSB.println(proximityStep);
    SerialUSB.print("loadCellStep: ");      SerialUSB.println(loadCellStep);
    SerialUSB.print("outputPosition: ");    SerialUSB.println(outputPosition);
    SerialUSB.print("outputDuration: ");    SerialUSB.println(outputDuration);
    SerialUSB.print("badOutputStep: ");     SerialUSB.println(badOutputStep);
    SerialUSB.print("badOutputPosition: "); SerialUSB.println(badOutputPosition);
    SerialUSB.print("badOutputDuration: "); SerialUSB.println(badOutputDuration);
    SerialUSB.print("homePlus: ");          SerialUSB.println(homePlus);
    SerialUSB.println("ENV VALUES .....END ");
}
