#ifndef EPOLL_POLLER_H_
#define EPOLL_POLLER_H_
#include"Poller.h"

struct epoll_event;
typedef Timer::Timestamp Timestamp;
class Epollpoller : public Poller
{
public:

    Epollpoller(EventLoop* loop);
    ~Epollpoller() override;

    Timestamp poll(int timeout,ChannelList* activelist) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;

private:

    typedef std::vector<struct epoll_event> EventList;
    EventList events_;
    int epollfd_;

    void fillActiveChannels(int numEvent,ChannelList* activelist);
    void update(int opetion,Channel* channel);
};














#endif
