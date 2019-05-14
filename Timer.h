#ifndef TIMER_H_
#define TIMER_H_
#include<time.h>
#include<functional>
#include<atomic>
#include<sys/time.h>
#include<assert.h>
extern const int kMicroSecondsPerSencond;
extern std::atomic<int> Atomic_num;
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
    ~Timer()=default;
    
    void run() const
    {
        assert(cb_);
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

    static Timestamp addTime(Timestamp timestamp,double seconds);
private:
    const Timercallback cb_;
    Timestamp expiration_;
    const bool repeat_;
    const double interval_;
    const int seq_;
    
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