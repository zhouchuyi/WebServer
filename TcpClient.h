#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include"Connector.h"
#include"base/noncopyable.h"
#include"TcpConnection.h"
#include"base/mutex.h"
#include<atomic>
class EventLoop;

class TcpClient:noncopyable
{
public:
    TcpClient(EventLoop* loop,const InetAddress& serverAddr,std::string name);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const
    { MutexLockGuard lock(mutex_); return conn_; }
    EventLoop* getLoop()const
    { return loop_; }

    bool retry()const
    { return retry_; }
    void enableRetry()
    { retry_=true; }
    const std::string& name()const
    { return name_; }
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectioncallback_=cb; }
    void setMessageCallback(const MessageCallback& cb)
    { messagecallback_=cb; }
private:
    void newConnection(int sockfd);
    
    void removeConnection(const TcpConnectionPtr& conn);
    EventLoop* loop_;
    TcpConnectionPtr connection_;
    std::string name_;
    ConnectionCallback connectioncallback_;
    MessageCallback messagecallback_;
    std::atomic<bool> retry_;
    std::atomic<bool> connect_;
    int nextConnId_;
    mutable MutexLock mutex_;
    TcpConnectionPtr conn_;
    std::shared_ptr<Connector> connector_;
};









#endif