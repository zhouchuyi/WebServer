#ifndef CHANNEL_H_
#define CHANNEL_H_

#include"base/noncopyable.h"
#include<functional>
#include<memory>

class EventLoop;

class Channel
{
public:
    typedef int64_t Timestamp; 
    typedef std::function<void()> EventCallback;
    // typedef std::function<void(Timestamp)> ReadEventCallback;//used to wake up loop
    
    Channel(EventLoop *loop,int fd);
    ~Channel();

    void handleEvent();
    //used for init
    void setReadCallback(EventCallback cb)
    {readCallback_=std::move(cb);}
    void setWriteCallback(EventCallback cb)
    {writeCallback_=std::move(cb);}
    void setCloseCallback(EventCallback cb)
    {closeCallback_=std::move(cb);}
    void setErrorCallback(EventCallback cb)
    {errorCallback_=std::move(cb);};

    int fd()const{return fd_;}
    int events()const{return events_;}
    void set_revents(int revt){revents_=revt;}
    bool isNoneEvent()const{return events_==kNoneEvent;}
    void EnableReading()
    {
        events_|=kReadEvent;
        update();
    }
    void disableReading()
    {
        events_&=~kReadEvent;
        update();
    }
    void EnableWritinging()
    {
        events_|=kWriteEvent;
        update();
    }
    void disableWriting()
    {
        events_&=~kWriteEvent;
        update();
    }
    void disableAll()
    {
        events_=kNoneEvent;
        update();
    }
    bool isWriting()const
    {
        return events_&kWriteEvent;
    }
    bool isReading()const
    {
        return events_&kReadEvent;
    }

    int indx()const
    {
        return index_;
    }
    void set_index(int idx)
    {
        index_=idx;
    }
    EventLoop *ownerLoop()
    {
        return loop_;
    }
    void remove();
    void tie(const std::shared_ptr<void> &obj);

private:
    void update();
    void handleEventWithGuard();
    static std::string eventsToString(int fd,int ev);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int  fd_;
    int        events_;
    int        revents_;
    int        index_;

    std::weak_ptr<void> tie_;
    bool tied_;
    bool eventHandling_;
    bool addedToLoop_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;


};





#endif