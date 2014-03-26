#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <unistd.h>

struct timeval timerStart, timerEnd;

void startTimer()
{
    gettimeofday(&timerStart, NULL);
}

void stopTimer()
{
    gettimeofday(&timerEnd, NULL);
}

double getLastTimerDuration()
{
    return (timerEnd.tv_sec  - timerStart.tv_sec +
            (timerEnd.tv_usec - timerStart.tv_usec) / 1000000.0);
}

#endif // TIMER_H
