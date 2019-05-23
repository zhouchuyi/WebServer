#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include"base/noncopyable.h"
#include"Buffer.h"
#include"net/InetAddress.h"

#include<memory>
#include<atomic>

class Channel;
class EventLoop;

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&,Buffer*)> MessageCallback;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void(const TcpConnectionPtr&)> WriteComleteCallback;
typedef std::function<void(const TcpConnectionPtr&,size_t)> HighWaterMarkCallback;

class TcpConnection:noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:

    TcpConnection(EventLoop* loop,const std::string name,int sockfd,const InetAddress& locla,const InetAddress &peer);
    ~TcpConnection();

    EventLoop* getLoop()const
    { return loop_; }
    const std::string& name()
    const{return name_; }
    const InetAddress& localAddress()const
    { return localAddr_; }
    const InetAddress& peerAddress()const
    { return peerAddr_; }
    bool connected()const
    { return state_==kConnected; }
    bool disconnected()const
    { return state_==kDisconnected; }

    void send(const void* message,size_t len);
    void send(const std::string& message);
    // void send(std::string&& message);
    void send(Buffer* message);
    // void send(Buffer&& mesaage);
    void shutdown();
    void setTcpNodelay(bool on);

    void startRead();
    void stopread();
    bool isreading()const{ return reading_; }

    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_=cb ;}
    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_=cb; }
    void setCloseCallback(const CloseCallback& cb)
    { closeCallback_=cb; }
    void setWriteComleteCallback(const WriteComleteCallback& cb)
    { writeCompleteCallback_=cb; }
    void setHighWaterMarkCallback(const HighWaterMarkCallback&cb,size_t high)
    { highWaterMarkCallback_=cb; highWaterMark_=high; }
    Buffer* inputBuffer()
    {
        return &inputBuffer_;
    }
    Buffer* outputBuffer()
    {
        return &outputBuffer_;
    }
    //called when server accept a new conn
    void connectEstablished();
    void connectDestroyed();
private:
    
    enum StateE{kDisconnected,kConnecting,kConnected,kDisconnecting};
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    // void sendInLoop(std::string &&message);
    // void sendInLoop(const std::string& message);
    void sendInLoop(const char* message,size_t len);
    void startReadInLoop();
    void stopReadInLoop();
    void shutdownInLoop();
    void setState(StateE s){ state_ = s ;}
    const char* stateToString()const;
    
    const std::string name_;
    EventLoop* loop_;
    bool reading_;
    // bool writting_;
    std::atomic<StateE> state_;    

    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Socket> socket_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    WriteComleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    size_t highWaterMark_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;

};






#endif