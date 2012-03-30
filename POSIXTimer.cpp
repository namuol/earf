#include <unistd.h> 
#include <sys/time.h>
#include <time.h>

#include "POSIXTimer.h"

void POSIXTimer::init()
{
    struct timeval tv;

    gettimeofday(&tv, NULL); 

    time_at_init = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void POSIXTimer::deinit()
{
    // Doesn't do anything.
}

int POSIXTimer::get_time()
{
    struct timeval tv;

    gettimeofday(&tv, NULL); 

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000) - time_at_init;
}

void POSIXTimer::lazy_wait(int ms)
{
    usleep(ms * 1000);
}
