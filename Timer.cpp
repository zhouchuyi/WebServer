#include"Timer.h"

const int kMicroSecondsPerSencond=1000*1000;

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