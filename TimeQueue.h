#ifndef TIMEQUEUE_H_
#define TIMEQUEUE_H_
#include"base/noncopyable.h"
#include"Timer.h"
#include<set>
#include<utility>
#include<vector>

class EventLoop;
class Channel;

class TimeQueue:noncopyable
{
private:
    typedef std::pair<Timer::Timestamp,Timer*> Entry;
    typedef std::set<Entry> TimerList;
    
    EventLoop *loop_;
    TimerList timers_;
    const int timerfd;
    Channel timerfdChannel_;
    std::vector<Entry> getExpired();
    void handleRead();
    
public:

    TimeQueue(EventLoop *loop);
    ~TimeQueue();


};




#endif