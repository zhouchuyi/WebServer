#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include"base/noncopyable.h"
#include"base/mutex.h"
#include"base/Thread.h"
#include"Timer.h"
#include<functional>
#include<vector>
class Channel;
class TimeQueue;
class Poller;

class EventLoop
{    

public:
    typedef Timer::Timestamp Timestamp;
    typedef std::function<void()> Functor;
    typedef Functor TimerCallback;
    EventLoop();
    ~EventLoop();

/* use loop() in the io thread */
    void loop();
/* better use sharedptr to call quit() */
    void quit();
/* run cb if in io thread 
push cb to queue and wake up io thread */
    void runInLoop(Functor cb);
    
    void queueInLoop(Functor cb);
    
    size_t queueSize() const;

    TimerId runAt(Timestamp time,TimerCallback cb);

    TimerId runAfter(double delay,TimerCallback cb);

    TimerId runEvery(double interval,TimerCallback cb);

    void cancel(TimerId timerId);
    void wakeup();

    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);
    

    void assertInLoopThread()
    {
        if(!isInLoopThread())
            abortNotInLoopThread();
    }

    bool isInLoopThread()const
    {
        return threadId_==CurrentThread::tid();
    }

    static EventLoop* getEventLoopOfCurrentThread();

    bool enentHandling() const
    {
        return eventHandling_;
    }

private:

    void abortNotInLoopThread();
    void handleRead();//wake up
    void doPendingFunctors();

    typedef std::vector<Channel*> ChannelList;
    
    const pid_t threadId_; 
    std::atomic<bool> looping_;//atomic
    std::atomic<bool> quit_;//atomic
    std::atomic<bool> eventHandling_;//atomic
    std::atomic<bool> callingPendingFunctors_;
    int64_t iteration_;
    Timestamp pollReturnTime;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<TimeQueue> timequeue_;
    int WakeupFd_;
    std::unique_ptr<Channel> wakeupChannel;

    ChannelList activechannels_;
    Channel * currentactivechannel_;
    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
};




#endif