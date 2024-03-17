#ifndef TASKS_H
#define TASKS_H

static const int led_pin = 17;

void videoReception(void *parameter);
void lockControl(void *parameter);
void dataTransmission(void *parameter);

#endif