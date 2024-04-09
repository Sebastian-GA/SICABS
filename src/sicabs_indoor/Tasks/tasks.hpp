#ifndef TASKS_H
#define TASKS_H

/*
    20 doesn't...
    19 doesn't...
    18 doesn't...
    17 works well.
    13 doesn't...
    12 works well
    11 doesn't...
*/

static const int ledPin = 17;
static const int buttonPin = 12;
static const int timeOpen = 5000;

void videoReception(void *parameter);
void lockControl(void *parameter);
void dataTransmission(void *parameter);

#endif