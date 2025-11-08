#include "cronometer.h"

Cronometer cronNew() {
    return (Cronometer){0, 0};
}

void cronStart(Cronometer* cron) {
    cron->base = clock();
}

void cronStop(Cronometer* cron) {
    clock_t end = clock();

    cron->millisec += (float)(end - cron->base)/CLOCKS_PER_SEC*1000;

}

void cronReset(Cronometer* cron) {
    cron->base = 0;
    cron->millisec = 0;
}