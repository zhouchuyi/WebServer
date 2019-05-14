#include"PollPoller.h"
#include"base/Logging.h"
#include"Channel.h"
#include<poll.h>
#include<algorithm>
PollPoller::PollPoller(EventLoop *loop)
 :Poller(loop)
 {

 }

PollPoller::~PollPoller()
{

}

Timestamp PollPoller::poll(int timeout,ChannelList *activelist)
{
    int numEvent=::poll(pollfds_.data(),pollfds_.size(),timeout);
    int savedErrno=errno;
    Timestamp nowtime=now();
    if(numEvent>0)
    {
        Log<<numEvent<<" events happened in PollPoller::poll()\n";
        fillActiveChannels(numEvent,activelist);     
    }
    else if (numEvent==0)
    {
        Log<<"no events happened in PollPoller::poll()\n";

    }
    else
    {
        if(savedErrno!=EINTR)
        {
            Log<<"error in PollPoller::poll()\n";
        }
    }
    
    return nowtime;

}

void PollPoller::updateChannel(Channel *channel)
{
    assertInLoopThread();
    // Log<<
    int idx=channel->indx();
    assert((idx>=0&&idx<=static_cast<int>(pollfds_.size()-1))||idx<0);
    if(idx>=0)
    {
        //channel is existing in map,update it in pollfds_
        assert(channels_.find(channel->fd())!=channels_.end());
        assert(channels_[channel->fd()]==channel);
        struct pollfd& pfd=pollfds_[idx];
        assert(pfd.fd==channel->fd()||pfd.fd==-channel->fd()-1);
        pfd.fd=channel->fd();
        pfd.events=static_cast<short>(channel->events());
        pfd.revents=0;
        if(channel->isNoneEvent())
        {
            //ignore it
            pfd.fd=-channel->fd()-1;
        }
    }
    else
    {
        //a new channel,add it into map and add a new poolfd into pollfds_
        assert(channels_.find(channel->fd())==channels_.end());
        struct pollfd pfd;
        pfd.events=static_cast<short>(channel->events());
        pfd.fd=channel->fd();
        pfd.revents=0;
        pollfds_.push_back(pfd);
        channel->set_index(static_cast<int>(pollfds_.size())-1);
        channels_[channel->fd()]=channel;
    }
//     if (channel->indx() < 0)
//   {
//     // a new one, add to pollfds_
//     assert(channels_.find(channel->fd()) == channels_.end());
//     struct pollfd pfd;
//     pfd.fd = channel->fd();
//     pfd.events = static_cast<short>(channel->events());
//     pfd.revents = 0;
//     pollfds_.push_back(pfd);
//     int idx = static_cast<int>(pollfds_.size())-1;
//     channel->set_index(idx);
//     channels_[pfd.fd] = channel;
//   }
//   else
//   {
//     // update existing one
//     assert(channels_.find(channel->fd()) != channels_.end());
//     assert(channels_[channel->fd()] == channel);
//     int idx = channel->indx();
//     assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
//     struct pollfd& pfd = pollfds_[idx];
//     assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd()-1);
//     pfd.fd = channel->fd();
//     pfd.events = static_cast<short>(channel->events());
//     pfd.revents = 0;
//     if (channel->isNoneEvent())
//     {
//       // ignore this pollfd
//       pfd.fd = -channel->fd()-1;
//     }
//   }
    
}

void PollPoller::fillActiveChannels(int numEvent,ChannelList *activelist)
{
    for (PollList::const_iterator pfd = pollfds_.begin(); pfd != pollfds_.end() && numEvent>0; ++pfd)
    {
        if(pfd->revents>0)
        {
            --numEvent;
            ChannelMap::const_iterator it=channels_.find(pfd->fd);
            assert(it!=channels_.end());
            Channel *activechannel=it->second;
            assert(activechannel->fd()==pfd->fd);
            activechannel->set_revents(static_cast<int>(pfd->revents));
            // pfd->revents=0;
            activelist->push_back(activechannel);

        }


    }
    
}

void PollPoller::removeChannel(Channel* channel)
{
    //remove channel from map and pollfds
    
    assertInLoopThread();
    
    //assert channels_
    assert(channels_.find(channel->fd())!=channels_.end());
    assert(channels_[channel->fd()]==channel);
    assert(channel->isNoneEvent());
    printf("%d %d\n",channel->fd(),channel->indx());
    //assert pollfds_
    int idx=channel->indx();
    assert(idx>=0 && idx<static_cast<int>(pollfds_.size()));
    struct pollfd& pfd=pollfds_[idx];
    assert(pfd.fd==-channel->fd()-1 && pfd.events==channel->events());
    
    //erase channel from channels_
    size_t n=channels_.erase(channel->fd());
    assert(n==1); (void)n;
    
    //pop_back from pollfds_
    if(static_cast<size_t>(idx)==pollfds_.size()-1)
    {
        pollfds_.pop_back();
    }
    else
    {
        int endFd=pollfds_.back().fd;
        if(endFd<0)
        {
            endFd=-endFd-1;
        }
        assert(channels_.find(endFd)!=channels_.end());
        channels_[endFd]->set_index(idx);
        std::iter_swap(pollfds_.begin()+idx,pollfds_.end()-1);
        pollfds_.pop_back();
    }
    
}