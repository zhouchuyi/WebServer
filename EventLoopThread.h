#ifndef EVENTLOOPTHREAD_H_
#define EVENTLOOPTHREAD_H_

#include"base/Thread.h"
#include"base/noncopyable.h"
#include"base/mutex.h"
class EventLoop;

class EventLoopThread:noncopyable
{    
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    
    explicit EventLoopThread(const std::string &name=std::string(),const ThreadInitCallback &cb=ThreadInitCallback());
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void ThreadFunc();

    EventLoop* loop_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    ThreadInitCallback initCallback_;

};

#endif