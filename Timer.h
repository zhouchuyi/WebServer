#ifndef TIMER_H_
#define TIMER_H_
#include<time.h>
#include<functional>
#include<atomic>
#include<sys/time.h>
static const int kMicroSecondsPerSencond;

class Timer
{
public:
    typedef std::function<void()> Timercallback;
    typedef int64_t Timestamp;
    
    Timer(Timercallback cb,Timestamp when,double interval)
     :cb_(std::move(cb)),
     expiration_(when),
     interval_(interval),
     repeat_(interval>0),
     seq_(++Atomic_num)
    {

    }
    ~Timer();
    
    void run() const
    {
        cb_();
    }

    bool repeat()const
    {
        return repeat_;
    }
    int64_t sequence()const
    {
        return seq_;
    }
    Timestamp expiration() const
    {
        return expiration_;
    }

    void restart(Timestamp now);

    static inline Timestamp addTime(Timestamp timestamp,double seconds);
private:
    const Timercallback cb_;
    Timestamp expiration_;
    const bool repeat_;
    const double interval_;
    const int seq_;
    static std::atomic<int> Atomic_num;
    
};

class TimerId
{

public:
    TimerId(Timer *timer,int seq)
     :timer_(timer),
     seq_(seq)
     {}
    TimerId()
     :timer_(NULL),
     seq_(0)
     {} 
    ~TimerId()=default;
private:
    friend class TimeQueue;
    Timer *timer_;
    int seq_;
};

Timer::Timestamp now();





#endif