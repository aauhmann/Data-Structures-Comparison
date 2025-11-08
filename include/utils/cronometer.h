#pragma once

#include <time.h>

typedef struct {
    clock_t base;
    float millisec;
} Cronometer;

Cronometer cronNew();
void cronStart(Cronometer* cron);
void cronStop(Cronometer* cron);
void cronReset(Cronometer* cron);