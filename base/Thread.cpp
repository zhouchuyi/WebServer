#include"Thread.h"
#include<pthread.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<stdio.h>
#include<unistd.h>
#include<string>
#include"CurrentThread.h"
pid_t gettid()
{
    return static_cast<pid_t>(syscall(SYS_gettid));
}
namespace CurrentThread
{
    __thread int t_cachedTid=0;
    __thread char t_tidString[32];
    __thread int t_threadStringLength=6;
    __thread const char * t_threadName="default";

}
void CurrentThread::cacheTid()
{
    if(t_cachedTid==0)
    {
        t_cachedTid=::gettid();
        t_tidStringLength=snprintf(t_tidString,sizeof(t_tidString),"%5d",t_cachedTid);
    }
}



struct ThreadData
{
    typedef Thread::ThreadFunc Func;
    Func func_;
    pid_t *tid_;
    CountDownLatch *latch_;
    std::string name_;
    ThreadData( Func func,
                const std::string &name,
                pid_t *tid,
                CountDownLatch *latch)
        : func_(std::move(func)),
          tid_(tid),
          name_(name),
          latch_(latch)
    {   }

    void runINThread()
    {
        *tid_=CurrentThread::tid();
        tid_=nullptr;
        latch_->CountDown();
        latch_=nullptr;
        CurrentThread::t_threadName=name_.empty()?"Thread":name_.c_str();
        func_();
    } 
};


void * startThread(void *arg)
{
    ThreadData *data=static_cast<ThreadData*>(arg);
    data->runINThread();
    delete data;
    return nullptr;

}

Thread::Thread(ThreadFunc func,const std::string &name)
: func_(std::move(func)),
name_(name),
started_(false),
joined_(false),
pthreadId_(0),
tid_(0),
latch_(1)
{
    setDefaultName();
}

Thread::~Thread()
{
    if(started_&&!joined_)
        pthread_detach(pthreadId_);
}
void Thread::setDefaultName()
{
    if(name_.empty())
    {
        char buf[32];
        snprintf(buf,sizeof(buf),"Thread");
        name_=buf;
    }
}
void Thread::start()
{
    assert(!started_);
    started_=true;
    ThreadData *data=new ThreadData(func_,name_,&tid_,&latch_);
    if(pthread_create(&pthreadId_,NULL,startThread,static_cast<void*>(data)))
    {
        started_=false;
        delete data;

    }
    else
    {
        latch_.wait();
        assert(tid_>0);
    }
    
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_=true;
    return pthread_join(pthreadId_,NULL);
}