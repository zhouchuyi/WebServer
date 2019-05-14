#ifndef TIMEQUEUE_H_
#define TIMEQUEUE_H_
#include"base/noncopyable.h"
#include"Timer.h"
#include"EventLoop.h"
#include"Channel.h"
#include<set>
#include<utility>
#include<vector>


class TimeQueue:noncopyable
{
private:
    typedef std::function<void()> TimerCallback;
    typedef std::pair<Timer::Timestamp,Timer*> Entry;//the expiration and the point of timer
    typedef std::pair<Timer*, int64_t> ActiveTimer;//the point of timer and seq
    typedef std::set<Entry> TimerList;
    typedef std::set<ActiveTimer> ActiveTimerSet;
    typedef Timer::Timestamp Timestamp; 

public:
    explicit TimeQueue(EventLoop *loop);
    ~TimeQueue();
    
    TimerId addTimer(TimerCallback cb,Timestamp when,double interval);
    void cancel(TimerId timerId);


private:
    
    EventLoop *loop_;
    TimerList timers_;
    ActiveTimerSet activeTimers_;//is same as timers_
    ActiveTimerSet cancelingTimers_;
    const int timerfd_;
    Channel timerfdChannel_;
    bool callingExpiredTimers_;

    //move out expired timers    
    std::vector<Entry> getExpired(Timestamp now);
    void addTimerInLoop(Timer *timer);
    void cancelInLoop(TimerId timerId);
    //called when timefd can be readed 
    void handleRead();
    void reset(const std::vector<Entry> & expired,Timestamp now);
    bool insert(Timer* timer);
    

};;




#endif