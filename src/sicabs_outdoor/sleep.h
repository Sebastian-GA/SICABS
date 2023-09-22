/**
 * @file sleep.h
 * @brief Sleep functions
 */

#ifndef SLEEP_H
#define SLEEP_H

#include <Arduino.h>

#include <sfm.hpp>

#include "common.h"
#include "config.h"

static TimerHandle_t timerToSleep = NULL;  // Timer to go to sleep
extern SFM_Module fingerprintSensor;

void setupSleep();
void goToSleep(TimerHandle_t xTimer);
void IRAM_ATTR motionDetected();
void IRAM_ATTR fingerprintInterrupt();

#endif