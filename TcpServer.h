#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include"base/noncopyable.h"
#include"TcpConnection.h"
#include<map>
#include<atomic>
class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer:noncopyable
{

public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    TcpServer(EventLoop* base,
              const InetAddress& listenAddr,
              const std::string& name,
              bool option=false);
    
    ~TcpServer();
    //start the server and listen
    void start();

    const std::string& name()const
    { return name_; }
    const std::string& ipPort()const
    {  }
    EventLoop* getLoop()const
    { return loop_; }

    void setThreadNum(int n);
    //for user
    void setThreadInitCallback(const ThreadInitCallback& cb)
    { initCallback_=cb; }
    //for user
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectioncallback_=cb; }
    //for user
    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_=cb; }
    

private:
    typedef std::map<std::string,TcpConnectionPtr> ConnectionMap;

    void newConnection(int sockfd,const InetAddress& peer);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);
    
    EventLoop* loop_;//loop for accept
    const std::string ipPort_;
    // InetAddress listenAddr_;
    std::string name_;
    
    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;
    
    ThreadInitCallback initCallback_;
    MessageCallback messageCallback_;
    ConnectionCallback connectioncallback_;
    ConnectionMap connections_;
    std::atomic<bool> started_;
    uint32_t nextConnId_;
};











#endif