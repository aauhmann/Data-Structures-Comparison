#include "cronometer.h"

static double qpFreq = 0;
static bool isFreqSet = 0;

Cronometer cronNew() {
    if (!isFreqSet) {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        qpFreq = f.QuadPart;
        isFreqSet = true;
    }

    Cronometer cron;
    cron.millisec = 0;
    cron.base.QuadPart = 0;
    return cron;
}

void cronStart(Cronometer* cron) {
    QueryPerformanceCounter(&cron->base);
}

void cronStop(Cronometer* cron) {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);

    cron->millisec += (end.QuadPart - cron->base.QuadPart)*TICKS_PER_SEC/qpFreq;

}

void cronReset(Cronometer* cron) {
    cron->base.QuadPart = 0;
    cron->millisec = 0;
}