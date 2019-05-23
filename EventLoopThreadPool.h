#ifndef EVENTLOOPTHREADPOLL_H_
#define EVENTLOOPTHREADPOLL_H_

#include"base/noncopyable.h"
#include"EventLoop.h"
#include"EventLoopThread.h"
#include<functional>
#include<memory>
#include<vector>
#include<string>



class EventLoopThreadPool:noncopyable
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;


    EventLoopThreadPool(EventLoop* base,const std::string & namearg);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads);
    void start(const ThreadInitCallback &cb=ThreadInitCallback());

    //round-robin
    EventLoop* getNextLoop();

   std:: vector<EventLoop*> getAllLoops();

    bool started()const
    {
        return started_;
    }

    const std::string& name()const
    {
        return name_;
    }
private:
    typedef std::unique_ptr<EventLoopThread> LoopThreadPtr;
    typedef std::vector<LoopThreadPtr> LoopThreadList;
    EventLoop* base_;
    std::string name_;
    bool started_;
    int next_;
    int numthreads_;
    LoopThreadList threads_;
    std::vector<EventLoop*> loops_;
   

};





#endif