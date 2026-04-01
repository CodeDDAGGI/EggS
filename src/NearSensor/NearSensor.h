#ifndef NEAR_SENSOR_H
#define NEAR_SENSOR_H

#include <Arduino.h>

void readNearSensor();
void nearSensorInit();
void syncPulse();
void eggExistSetting();
void syncPulseTimer();

#endif // NEAR_SENSOR_H