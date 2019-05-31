#include"Epollpoller.h"
#include"base/Logging.h"
#include"Channel.h"
#include<sys/epoll.h>
#include<poll.h>
#include<assert.h>
#include<unistd.h>

namespace 
{
    const int kInitEventListSize=16;    
    const int kAdded=1;
    const int kNew=-1;
    const int kDeleted=2;
} // namespace 


Epollpoller::Epollpoller(EventLoop* loop)
 :Poller(loop),
  epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
  events_(kInitEventListSize)
  {
      if(epollfd_<0)
      {
          Log<<"Epollpoller::Epollpoller create error ";
      }
  }


Epollpoller::~Epollpoller()
{
    ::close(epollfd_);
}

Timestamp Epollpoller::poll(int timeout,ChannelList* activelist)
{
    int numEvent=::epoll_wait(epollfd_,events_.data(),events_.size(),timeout);
    int savedErrno=errno;
    Timestamp nowtime=now();
    if(numEvent>0)
    {
        Log<<numEvent<<" event happend in Epollpoller::poll() ";
        fillActiveChannels(numEvent,activelist);
        if(numEvent==events_.size())
        {
            events_.resize(numEvent*2);
        }
    }
    else if (numEvent==0)
    {
        Log<<" nothing happend in Epollpoller::poll() ";
    }
    else
    {
        if(savedErrno!=EINTR)
        {
            Log<<" error in Epollpoller::poll() ";
        }
    }
}


void Epollpoller::fillActiveChannels(int numEvent,ChannelList* activelist)
{
    assert(static_cast<size_t>(numEvent) <= events_.size());
    for (int i = 0; i < numEvent; i++)
    {
        Channel* channel=static_cast<Channel*>(events_[i].data.ptr);
        int fd=channel->fd();
        auto it=channels_.find(fd);
        assert(it!=channels_.end());
        assert(it->second==channel);
        channel->set_revents(events_[i].events);
        activelist->push_back(channel);
    }
}

void Epollpoller::update(int operation, Channel* channel)
{
    struct epoll_event event;
    memset(&event,0,sizeof event);
    event.data.ptr=channel;
    event.events=channel->events();
    int fd=channel->fd();
    if(::epoll_ctl(epollfd_,operation,fd,&event)<0)
    {
        perror("error in Epollpoller::update:: ");
        Log<<" error in Epollpoller::update ";
    }

}


void Epollpoller::updateChannel(Channel* channel)
{
    assertInLoopThread();
    const int indx=channel->indx();
    if( indx==kNew || indx==kDeleted )
    {
        //update a new one
        int fd=channel->fd();
        if(indx==kNew)
        {
            assert(channels_.find(fd)==channels_.end());
            channels_[fd]=channel;

        }
        else
        {
            assert(channels_.find(fd)!=channels_.end());
            assert(channels_[fd]==channel);
        }
        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD,channel);
    }
    else
    {
        int fd=channel->fd();
        assert(channels_.find(fd)!=channels_.end());
        assert(channels_[fd]==channel);
        assert(indx==kAdded);
        if(channel->isNoneEvent())
        {
            channel->set_index(kDeleted);
            update(EPOLL_CTL_DEL,channel);
        }
        else
        {
            update(EPOLL_CTL_MOD,channel);
        }
        
    }
    
}

void Epollpoller::removeChannel(Channel* channel)
{
    assertInLoopThread();
    const int indx=channel->indx();
    assert( indx==kAdded || indx==kDeleted );
    int fd=channel->fd();
    assert(channels_.find(fd)!=channels_.end());
    assert(channels_[fd]==channel);
    assert(channel->isNoneEvent());
    size_t n=channels_.erase(fd);
    assert(n==1);
    if(indx==kAdded)
        update(EPOLL_CTL_DEL,channel);
    channel->set_index(kNew);

}