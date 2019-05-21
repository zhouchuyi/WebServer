#include"EventLoopThreadPool.h"




EventLoopThreadPool::EventLoopThreadPool(EventLoop* base,std::string & namearg)
 : base_(base),
   name_(namearg),
   started_(false),
   next_(0),
   numthreads_(1)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{

}

void EventLoopThreadPool::start(const ThreadInitCallback &cb)
{
    assert(!started_);
    base_->assertInLoopThread();
    started_=true;
    for (int i = 0; i < numthreads_; i++)
    {
        char name[name_.size()+32];
        snprintf(name,sizeof name,"%s%u",name_.c_str(),i);
        EventLoopThread *t=new EventLoopThread(name,cb);
        threads_.push_back(LoopThreadPtr(t));
        loops_.push_back(t->startLoop());
    }
    if(numthreads_==0 && cb)
    {
        cb(base_);
    }    



}

void EventLoopThreadPool::setThreadNum(int numThreads)
{
    numthreads_=numThreads;
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    assert(started_);
    base_->assertInLoopThread();

    EventLoop* loop=base_;
    if(!loops_.empty())
    {
        loop=loops_[next_];
        ++next_;
        if(static_cast<size_t>(next_)==loops_.size())
        {
            next_ = 0;
        }
    }

    return loop;
}