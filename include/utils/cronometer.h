#pragma once

#include <windows.h>

#define TICKS_PER_SEC 1000

typedef struct {
    LARGE_INTEGER base;
    double millisec; 
} Cronometer;

Cronometer cronNew();
void cronStart(Cronometer* cron);
void cronStop(Cronometer* cron);
void cronReset(Cronometer* cron);