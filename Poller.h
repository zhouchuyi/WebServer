#ifndef POLLER_H_
#define POLLER_H_
#include"EventLoop.h"
#include<map>
class Poller:noncopyable
{
public:
    typedef std::vector<Channel*> ChannelList;
    typedef int64_t Timestamp;
    Poller(EventLoop *);

    virtual ~Poller();

    virtual Timestamp poll(int timeoutMs,ChannelList *activeChannels)=0;

    virtual void updateChannel(Channel *)=0;

    virtual void removeChannel(Channel *)=0;

    virtual bool hasChannel(Channel *) const;
    
    void assertInLoopThread() const
    {
        ownerLoop_->assertInLoopThread();
    } 

    static Poller* newDefaultPoller(EventLoop *loop);
protected:
    typedef std::map<int,Channel*> ChannelMap;//the map between fd and channel
    ChannelMap channels_;

private:
    EventLoop* ownerLoop_;//owner loop
};



#endif