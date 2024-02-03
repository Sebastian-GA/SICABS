#ifndef TASKS_H
#define TASKS_H

static const int led_pin = 17;

// This task receives video from the ESP32-CAM and projects it on the screen
void videoProjection(void *parameter);

// This tasks blinks a simple LED
void blinkLED(void *parameter);

#endif