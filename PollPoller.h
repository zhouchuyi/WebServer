#ifndef POLL_POLLER_H_
#define POLL_POLLER_H_
#include"Poller.h"

struct pollfd;
typedef Timer::Timestamp Timestamp;
class PollPoller : public Poller
{
public:
    
    PollPoller(EventLoop *loop);
    ~PollPoller() override;

    Timestamp poll(int timeout,ChannelList *activelist);
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;


private:
    typedef std::vector<struct pollfd> PollList;

    PollList pollfds_;

    void fillActiveChannels(int numEvent,ChannelList *activelist);

};








#endif

