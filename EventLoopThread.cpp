#include"EventLoopThread.h"
#include"EventLoop.h"


EventLoopThread::EventLoopThread(const std::string& name,const ThreadInitCallback &cb)
: loop_(nullptr),
  mutex_(),
  cond_(mutex_),
  thread_(std::bind(&EventLoopThread::ThreadFunc,this),name),
  initCallback_(cb)
  {

  }
EventLoopThread::~EventLoopThread()
{
    if(loop_!=nullptr)
    {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop()
{
    assert(!thread_.started());
    EventLoop* loop=nullptr;
    thread_.start();
    {
        MutexLockGuard lock(mutex_);
        while (loop_==nullptr)
        {
            cond_.wait();
        }
        loop=loop_;
    }

    return loop;

}


void EventLoopThread::ThreadFunc()
{
    EventLoop loop;
    if(initCallback_)
    {
        initCallback_(&loop);
    }
    
    {
        MutexLockGuard lock(mutex_);
        loop_=&loop;
        cond_.notify();
    }

    loop.loop();

    MutexLockGuard lock(mutex_);
    loop_=nullptr;

}