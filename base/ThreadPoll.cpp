#include"ThreadPoll.h"

ThreadPoll::ThreadPoll(const std::string&name)
: name_(name),
mutex_(),
notEmpty_(mutex_),
notFull_(mutex_),
threads_(),
queue_(),
maxQueueSize_(0),
running_(false)
{

}

ThreadPoll::~ThreadPoll()
{
    if(running_)
    {
        stop();
    }
}

void ThreadPoll::stop()
{
    {
        MutexLockGuard lock(mutex_);
        running_=false;
        notEmpty_.notifyAll();//wake up threads block on taking task from queue_;
    }
    for(auto& thread : threads_)
    {
        thread->join();
    }

    
}

void ThreadPoll::start(int numThread)
{
    assert(threads_.empty());
    threads_.reserve(numThread);
    char id[32];
    for(size_t i = 0; i < numThread; i++)
    {
        snprintf(id,sizeof id,"%5d",i);
        threads_.emplace_back(
            std::bind(&ThreadPoll::runInthread,this),name_+id);
        threads_[i]->start();
    }
    if(numThread==0 && threadInitCallback_)
    {
        threadInitCallback_();
    }
    
}

void ThreadPoll::runInthread()
{
    if(threadInitCallback_)
    {
        threadInitCallback_();
    }
    while(running_)
    {
        Task task(std::move(take()));
        if(task)
        {
            task();
        }
    }
    
}

size_t ThreadPoll::QueueSize() const
{
    MutexLockGuard lock(mutex_);
    return queue_.size();
}

bool ThreadPoll::isFull() const
{
    mutex_.assertLocked();
    return queue_.size()>=maxQueueSize_ && maxQueueSize_>0;
}

std::function<void ()> ThreadPoll::take()
{
    MutexLockGuard lock(mutex_);
    while(running_ && queue_.empty())
    {
        notEmpty_.wait();
    }
    std::function<void ()> task;
    if(!queue_.empty())
    {
        task=std::move(queue_.front());
        queue_.pop_front();
        notFull_.notify();
    }        
    return task;
}

void ThreadPoll::run(Task tsk)
{
    if(threads_.empty())
    {
        tsk();
    }
    MutexLockGuard lock(mutex_);
    while(isFull())
    {
        notFull_.wait();
    }
    
    assert(!isFull());
    queue_.push_back(std::move(tsk));
    notEmpty_.notify();
}
