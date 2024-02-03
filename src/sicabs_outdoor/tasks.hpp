#ifndef TASKS_H
#define TASKS_H

// This task receives video from the ESP32-CAM and projects it on the screen
void videoTransmission(void *parameter);

// This tasks transmits through ESP NOW
void espNOW(void *parameter);

#endif