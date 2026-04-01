#ifndef SYSTEM_UTIL_H
#define SYSTEM_UTIL_H

#include <Arduino.h>

void integerToBytes(long val, byte b[2]);
void intArrayToByteArray();
void bytesToIntegerArray();
void getNowSequence();
void shiftLoadCellArray();
void reboot();
void extLedOn();
void extLedOff();

#endif 
