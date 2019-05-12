#include"TimeQueue.h"
#include"base/Logging.h"
#include<sys/timerfd.h>
#include<unistd.h>
#include<algorithm>


int createTimerfd()
{
    int timefd=timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
    if(timefd<0)
    {
        Log<<"Failed in createTimerfd\n";
    }
    return timefd;
}

void readTimerfd(int timerfd,Timer::Timestamp now)
{
    uint64_t howmany;//the  number  of  expirations  that  have  occurred.
    ssize_t n=read(timerfd,&howmany,sizeof howmany);
    Log<<"TimerQueue::handleRead() "<<howmany<<"\n";
    if(n!=sizeof howmany)
    {
        Log<<"TimerQueue::handleRead() read "<<n<<"bytes\n";
    }

}

struct timespec howMuchTimeFromNow(Timer::Timestamp when)
{
    Timer::Timestamp microseconds=when-now();
    if(microseconds<100)
    {
        microseconds=100;
    }
    struct timespec ts;
    ts.tv_sec=static_cast<time_t>(microseconds/kMicroSecondsPerSencond);
    ts.tv_nsec=static_cast<long>(microseconds%kMicroSecondsPerSencond*1000);
    return ts;
}

void resetTimerfd(int timefd,Timer::Timestamp expiration)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memset(&newValue,0,sizeof newValue);
    memset(&oldValue,0,sizeof oldValue);
    newValue.it_value=howMuchTimeFromNow(expiration);
    int ret=timerfd_settime(timefd,0,&newValue,&oldValue);
    if(ret)
    {
        Log<<"timerfd settime() error\n";
    }

}
TimeQueue::TimeQueue(EventLoop *loop)
 :  loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop_,timerfd_),
    callingExpiredTimers_(false),
    timers_(),
    activeTimers_(),
    cancelingTimers_()
    {

    }

TimeQueue::~TimeQueue()
{
    timerfdChannel_.disableAll();
    timerfdChannel_.remove();
    close(timerfd_);
    for (auto &timer : timers_)
    {
        delete timer.second;
    }
        
}
TimerId TimeQueue::addTimer(TimerCallback cb,Timestamp when,double interval)
{
    Timer *timer=new Timer(std::move(cb),when,interval);
    loop_->runInLoop(std::bind(&TimeQueue::addTimerInLoop,this,timer));
    return TimerId(timer,timer->sequence());
}

void TimeQueue::cancel(TimerId timerId)
{
    loop_->runInLoop(std::bind(&TimeQueue::cancelInLoop,this,timerId));
}

void TimeQueue::addTimerInLoop(Timer *timer)
{
    loop_->assertInLoopThread();
    bool earliestChanged=insert(timer);
    if(earliestChanged)
    {
        resetTimerfd(timerfd_,timer->expiration());
    }

}

void TimeQueue::cancelInLoop(TimerId timerId)
{
    loop_->assertInLoopThread();
    assert(timers_.size()==activeTimers_.size());
    ActiveTimer actimer(timerId.timer_,timerId.seq_);    
    ActiveTimerSet::iterator it=activeTimers_.find(actimer);
    if(it!=activeTimers_.end())
    {
        ssize_t n=timers_.erase(Entry(it->first->expiration(),it->first));
        assert(n==1);
        delete it->first;
        activeTimers_.erase(it);
    }
    else if (callingExpiredTimers_)
    {
        cancelingTimers_.insert(actimer);
    }
    assert(timers_.size()==activeTimers_.size());

}

bool TimeQueue::insert(Timer * timer)
{
    loop_->assertInLoopThread();
    assert(activeTimers_.size()==timers_.size());
    bool earliestChanged=false;
    Timestamp when=timer->expiration();
    TimerList::iterator it=timers_.begin();
    if(it==timers_.end()||when<it->first)
    {
        earliestChanged=true;
    }
    {
        auto result=timers_.insert(Entry(when,timer));
        assert(result.second);(void)result;
    }
    {
        auto result=activeTimers_.insert(ActiveTimer(timer,when));
        assert(result.second);(void)result;
    }
    assert(activeTimers_.size()==timers_.size());
    return earliestChanged;
}


void TimeQueue::handleRead()
{
    loop_->assertInLoopThread();
    Timestamp nowtime=now();
    readTimerfd(timerfd_,nowtime);
    std::vector<Entry> expired=getExpired(nowtime);
    callingExpiredTimers_=true;
    cancelingTimers_.clear();
    for (auto &i : expired)
    {
        i.second->run();

    }
    callingExpiredTimers_=false;
    
    reset(expired,nowtime);
}

std::vector<std::pair<Timer::Timestamp,Timer*>> TimeQueue::getExpired(Timestamp now)
{
    assert(timers_.size()==activeTimers_.size());
    Entry sentry=std::make_pair(now,reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator it=std::lower_bound(timers_.begin(),timers_.end(),sentry);
    assert(it==timers_.end()||now<it->first);
    std::vector<Entry> expired(timers_.begin(),it);
    timers_.erase(timers_.begin(),it);
    for (const Entry &i : expired)
    {
        ActiveTimer actimer(i.second,i.second->sequence());
        ssize_t n=activeTimers_.erase(actimer);
        assert(n==1);
    }
    assert(timers_.size()==activeTimers_.size());
    return expired;

}

void TimeQueue::reset(const std::vector<Entry> & expired,Timestamp now)
{
    Timestamp nextexpire;

    for (auto &i : expired)
    {
        ActiveTimer actimer(i.second,i.second->sequence());
        if(i.second->repeat()&&cancelingTimers_.find(actimer)==cancelingTimers_.end())
        {
            i.second->restart(now);
            insert(i.second);
        }
        else
        {
            delete i.second;
        }
    }
    if(!timers_.empty())
    {
        nextexpire=(timers_.begin())->second->expiration();
    }
    if(nextexpire>0)
    {
        resetTimerfd(timerfd_,nextexpire);
    }
    
}
