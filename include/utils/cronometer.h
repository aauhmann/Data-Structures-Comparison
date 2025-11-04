#pragma once

#include <time.h>

#define CLOCKS_PER_SEC 1000

typedef struct {
    clock_t base;
    float millisec;
} Cronometer;

Cronometer cronNew();
void cronStart(Cronometer* cron);
void cronStop(Cronometer* cron);