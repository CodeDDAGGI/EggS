#ifndef FLASH_ROM_H
#define FLASH_ROM_H

#include <Arduino.h>
#include <DueFlashStorage.h>

extern DueFlashStorage dueFlashStorage;

// FLASH에서 데이터 읽기
bool Read_FlashData();

// FLASH에 데이터 저장
void Save_FlashData();

// LED 배열 저장
void saveLedArray_FlashData();

// LED 배열 읽기
void getLedArray_FlashData();

// 설정값 출력
void print_FlashData();

#endif
