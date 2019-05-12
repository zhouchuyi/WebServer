#include"EventLoop.h"
#include"Channel.h"
#include"base/Logging.h"
#include<poll.h>
#include<assert.h>
#include<sstream>

const int Channel::kNoneEvent=0;
const int Channel::kReadEvent=POLLIN|POLLPRI;
const int Channel::kWriteEvent=POLLOUT;

Channel::Channel(EventLoop *loop,int fd)
  : loop_(loop),
    fd_(fd),
    events_(kNoneEvent),
    revents_(kNoneEvent),
    index_(-1),
    tie_(),
    tied_(false),
    eventHandling_(false),
    addedToLoop_(false)
    {

    }

Channel::~Channel()
{
    assert(!eventHandling_);
    assert(!addedToLoop_);
    if(loop_->isInLoopThread())
    {
        assert(!loop_->hasChannel(this));
    }
}

void Channel::update()
{
    addedToLoop_=true;
    loop_->updateChannel(this);
}

void Channel::tie(const std::shared_ptr<void> &obj)
{
    tied_=true;
    tie_=obj;
}

void Channel::remove()
{
    assert(isNoneEvent());
    addedToLoop_=false;
    loop_->removeChannel(this);
}

void Channel::handleEvent()
{
    std::shared_ptr<void> ptr;
    if(tied_)
    {
        ptr=tie_.lock();
        if(ptr)
        {
            handleEventWithGuard();
        }
    }
    else
    {
        handleEventWithGuard();
    }
    
}

void Channel::handleEventWithGuard()
{
    eventHandling_=true;
    Log<<eventsToString(fd_,revents_);
    if(revents_&POLLNVAL)
        Log<<" WARNING Channel::handleEvent() POLLNVAL"<<"\n";
    if(revents_&(POLLERR|POLLNVAL))
    {
        if(errorCallback_)
            errorCallback_();
    }
    if(revents_&(POLLIN|POLLPRI|POLLHUP))
    {
        if(readCallback_)
            readCallback_();
    }
    if(revents_&(POLLOUT))
    {
        if(writeCallback_)
            writeCallback_();
    }



    eventHandling_=false;


}

std::string Channel::eventsToString(int fd,int ev) 
{
    std::ostringstream oss;
    oss<<fd<<": ";
    if(ev&POLLIN)
        oss<<"IN ";
    if(ev&POLLPRI)
        oss<<"PRI ";
    if(ev&POLLOUT)
        oss<<"OUT ";
    if(ev&POLLHUP)
        oss<<"HUP ";
    if (ev & POLLERR)
        oss << "ERR ";
    if (ev & POLLNVAL)
        oss << "NVAL ";
    return oss.str();
}