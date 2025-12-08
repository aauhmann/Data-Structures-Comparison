#include "chronometer.h"
#include <stdbool.h>

static double qpFreq = 0;
static bool isFreqSet = 0;

Chronometer chronNew() {
    if (!isFreqSet) {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        qpFreq = f.QuadPart;
        isFreqSet = true;
    }

    Chronometer cron;
    cron.millisec = 0;
    cron.base.QuadPart = 0;
    return cron;
}

void chronStart(Chronometer* cron) {
    QueryPerformanceCounter(&cron->base);
}

void chronStop(Chronometer* cron) {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);

    cron->millisec += (end.QuadPart - cron->base.QuadPart)*TICKS_PER_SEC/qpFreq;

}

void chronReset(Chronometer* cron) {
    cron->base.QuadPart = 0;
    cron->millisec = 0;
}