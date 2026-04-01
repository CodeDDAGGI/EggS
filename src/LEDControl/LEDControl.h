#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Arduino.h>
#include <Wire.h>

void I2C_finder(); 
void ledON();
void ledOff();
void ledEachOff();
void ledManualON(byte ledLight1, byte ledLight2, byte ledLight3, byte ledLight4 , byte ledLight5);
void handleManualLED(byte ledBits , int Index , bool OnOff); // 새로 추가한 함수 250418
void ledManualOff(byte ledLight1, byte ledLight2, byte ledLight3, byte ledLight4 , byte ledLight5);
void ledONTimer();
void ledOffTimer();
void ledBrightList();
void ledDurationList();
void ledSetBright(int value);
void setLEDarray(int type);
void processLED(byte ledBits, int startIndex);

#endif // LEDCONTROL_H
