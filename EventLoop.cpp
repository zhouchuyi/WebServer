#include"EventLoop.h"
#include"poll.h"
#include"TimeQueue.h"
#include"PollPoller.h"
#include"base/Logging.h"
#include<algorithm>
#include<sys/eventfd.h>
#include<unistd.h>
#include<signal.h>
namespace 
{
    __thread EventLoop* t_loopInthisThread=nullptr;
    const int kPollTimeMs=10000;
    //create eventfd
    int createEventfd()
    {
        int fd=::eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC); 
        if(fd<0)
        {
            Log<<"error in creating eventfd\n";
            abort();
        }
        return fd;
    }
    //ignore signal
    
} // namespace 

EventLoop::EventLoop()
 :  looping_(false),
    quit_(false),
    eventHandling_(false),
    callingPendingFunctors_(false),
    iteration_(0),
    threadId_(CurrentThread::tid()),
    pollReturnTime(0),
    poller_(Poller::newEpoller(this)),
    timequeue_(new TimeQueue(this)),
    WakeupFd_(::createEventfd()),
    wakeupChannel(new Channel(this,WakeupFd_)),
    activechannels_(),
    currentactivechannel_(nullptr),
    pendingFunctors_(),
    mutex_()
    {
        assert(t_loopInthisThread==nullptr);
        t_loopInthisThread=this;
        wakeupChannel->setReadCallback(std::bind(&EventLoop::handleRead,this));
        wakeupChannel->enableReading();
    }

EventLoop::~EventLoop()
{
    t_loopInthisThread=nullptr;
    wakeupChannel->disableAll();
    wakeupChannel->remove();
}

void EventLoop::loop()
{
    assertInLoopThread();
    assert(!looping_);
    looping_=true;
    quit_=false;
    while (!quit_)
    {
        ++iteration_;
        activechannels_.clear();
        pollReturnTime=poller_->poll(kPollTimeMs,&activechannels_);
        eventHandling_=true;
        for (ChannelList::const_iterator it = activechannels_.begin(); it != activechannels_.end(); it++)
        {
            currentactivechannel_=(*it);
            (*it)->handleEvent();
            currentactivechannel_=nullptr;

        }
        eventHandling_=false;
        doPendingFunctors();

    }
    looping_=false;
    
}
void EventLoop::quit()
{
    quit_=true;
    if(!isInLoopThread())
    {
        wakeup();
    }

}
void EventLoop::runInLoop(Functor cb)
{
    if(isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(std::move(cb));
    }
    
}
void EventLoop::queueInLoop(Functor cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(std::move(cb));
    }
    //wake up if not in IO thread or callingpending
    if(!isInLoopThread()||callingPendingFunctors_)
    {
        wakeup();
    }
}

size_t EventLoop::queueSize() const
{
    MutexLockGuard lock(mutex_);
    return pendingFunctors_.size();
}

void EventLoop::wakeup()
{
    uint64_t one=1;
    ssize_t n=::write(WakeupFd_,&one,sizeof one);
    if(n!=sizeof one)
    {
        Log<<"error in wake up \n";
    }
}
void EventLoop::handleRead()
{
    uint64_t one;
    ssize_t n=::read(WakeupFd_,&one,sizeof one);
    if(n!=sizeof one)
    {
        Log<<"error in read wakeupfd_\n";
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop()==this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    assert(channel->ownerLoop()==this);
    assertInLoopThread();
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel)
{
    assert(channel->ownerLoop()==this);
    assertInLoopThread();
    return poller_->hasChannel(channel);
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInthisThread;
}

void EventLoop::abortNotInLoopThread()
{
    Log<<"EventLoop has created in thread "<<CurrentThread::tid()<<"\n";
    abort();
}

void EventLoop::doPendingFunctors()
{
    callingPendingFunctors_=true;
    std::vector<Functor> Functors;
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.swap(Functors);
    }
    for (auto &func : Functors)
    {
        func();
    }
    callingPendingFunctors_=false;
    
}

TimerId EventLoop::runAt(Timestamp time, TimerCallback cb)
{
    return timequeue_->addTimer(std::move(cb),time,0);
}

TimerId EventLoop::runAfter(double delay,TimerCallback cb)
{
    Timestamp when=Timer::addTime(now(),delay);
    return timequeue_->addTimer(std::move(cb),when,0);
}

TimerId EventLoop::runEvery(double interval,TimerCallback cb)
{
    Timestamp when=Timer::addTime(now(),interval);
    return timequeue_->addTimer(std::move(cb),when,interval);
}