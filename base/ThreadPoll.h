#ifndef BASE_THREADPOLL_H_
#define BASE_THREADPOLL_H_
#include"mutex.h"
#include"Condition.h"
#include"Thread.h"
#include<memory>
#include<vector>
#include<functional>
#include<deque>
class ThreadPoll:noncopyable
{
private:
    typedef std::unique_ptr<Thread> ThreadPtr;
    typedef std::vector<ThreadPtr> ThreadVectors;
    typedef std::function<void()> Task;
public:
    ThreadPoll(const std::string &name=std::string("ThreadPoll"));
    ~ThreadPoll();

    void setMaxQueueSize(int maxSize)
    {maxQueueSize_=maxSize;}
    void setThreadInitCallback(const Task &tsk)
    {threadInitCallback_=tsk;}

    void start(int numThread);
    void stop();
    void run(Task tsk);
    
    const std::string & name() const
    {return name_;} 
    size_t QueueSize() const;

private:

    Task threadInitCallback_;
    std::string name_;
    ThreadVectors threads_;
    mutable MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    std::deque<Task> queue_;
    size_t maxQueueSize_;
    bool running_;
    
    
    Task take();//take task from queue;
    void runInthread();//call take() and run task
    bool isFull()const;
};






#endif