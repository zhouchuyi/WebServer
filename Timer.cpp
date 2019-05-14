#include"Timer.h"
#include<assert.h>
const int kMicroSecondsPerSencond=1000*1000;
std::atomic<int> Atomic_num(0);


Timer::Timestamp Timer::addTime(Timestamp timestamp,double seconds)
{
    Timestamp delta=static_cast<int64_t>(seconds*kMicroSecondsPerSencond);
    return delta+timestamp;
}

void Timer::restart(Timestamp now)
{
    assert(repeat_);
    expiration_=addTime(now,interval_);
}

Timer::Timestamp now()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    Timer::Timestamp seconds=tv.tv_sec;
    return static_cast<Timer::Timestamp>(seconds*kMicroSecondsPerSencond+tv.tv_usec);
    
}