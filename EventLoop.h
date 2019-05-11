#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include"base/noncopyable.h"
#include"base/mutex.h"
#include"base/Thread.h"
#include"Timer.h"
#include<functional>
class Channel;
class TimeQueue;
class poll;

class EventLoop
{    

public:
    typedef int64_t Timestamp;
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

    void wakeup();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    void hasChannel(Channel *channel);
    
    void assertInLoopThread();

    bool isInLoopThread()const
    {
        return threadId_==CurrentThread::tid();
    }

private:

    void abortNotInLoopThread();
    void handleRead();//wake up
    void doPendingFunctors();

    typedef std::vector<Channel*> ChannelList;
    const pid_t threadId_; 
    std::atomic<bool> looping_;
    std::atomic<bool> quit_;

};




#endif