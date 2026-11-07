#pragma once

#include <windows.h>

#define TICKS_PER_SEC 1000

typedef struct {
    LARGE_INTEGER base;
    double millisec; 
} Chronometer;

Chronometer chronNew();
void chronStart(Chronometer* chron);
void chronStop(Chronometer* chron);
void chronReset(Chronometer* chron);