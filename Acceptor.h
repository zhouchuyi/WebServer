#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include"base/noncopyable.h"
#include"EventLoop.h"
#include"net/InetAddress.h"
#include"Channel.h"


class Acceptor:noncopyable
{

public:
    typedef std::function<void(int sockfd,const InetAddress&)> NewConnectionCallback;


    Acceptor(EventLoop* loop,const InetAddress &listenAddr);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback &cb)
    {
        newConnectionCallback_=cb;
    }

    bool isListening() const
    {
        return listenning_;
    }

    void listen();
private:
    EventLoop *loop_;
    Channel acceptChannel_;    
    Socket listenSocket_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
    void handleRead();

    int idleFd_;

};




#endif