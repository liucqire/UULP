#include <sys/time.h>
#include <stdlib.h>

void set_ticker(int t) {
    struct itimerval itv;
    int sec = t / 1000;
    int usec = (t % 1000) * 1000;
    itv.it_value.tv_sec = sec;
    itv.it_value.tv_usec = usec;
    itv.it_interval.tv_sec = sec;
    itv.it_interval.tv_usec = usec;
    setitimer(ITIMER_REAL, &itv, NULL);
}
