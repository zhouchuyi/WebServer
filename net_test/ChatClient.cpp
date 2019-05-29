#include"../Chat/codec.h"
#include"../EventLoop.h"
#include"../TcpClient.h"
#include"../EventLoopThread.h"
#include<iostream>
using namespace std::placeholders;

class ChatClient:noncopyable
{
public:
    ChatClient(EventLoop* loop,const InetAddress& serveraddr)
     : client_(loop,serveraddr,"ChatClient"),
       codec_(std::bind(&ChatClient::onStringMessage,this,_1,_2)),
       mutex_()
     {
         client_.setConnectionCallback(std::bind(&ChatClient::ConnectionCallback,this,_1));
         client_.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage,&codec_,_1,_2));
     }
    ~ChatClient()=default;
    void send(const std::string&);
    void strat()
    { client_.connect(); }
private:
    void ConnectionCallback(const TcpConnectionPtr&);
    void onStringMessage(const TcpConnectionPtr&,const std::string&);
    TcpClient client_;
    LengthHeaderCodec codec_;
    TcpConnectionPtr TcpConnectiton_;
    MutexLock mutex_;
};

void ChatClient::send(const std::string& mess)
{
    MutexLockGuard lock(mutex_);
    assert(TcpConnectiton_);
    codec_.send(TcpConnectiton_,mess);
}
void ChatClient::ConnectionCallback(const TcpConnectionPtr& conn)
{
    Log<<conn->peerAddress().toIpPort()<<" -> "
       <<conn->localAddress().toIpPort()
       <<(conn->connected() ? " ON " : " OFF ");
    MutexLockGuard lock(mutex_);
    if(conn->connected())
    {
        assert(!TcpConnectiton_);
        TcpConnectiton_=conn;
    }
    else
    {
        assert(TcpConnectiton_);
        TcpConnectiton_.reset();
        assert(!TcpConnectiton_);
    }
}
void ChatClient::onStringMessage(const TcpConnectionPtr& conn,const std::string& mess)
{
    printf("%s :%s\n",conn->peerAddress().toIp().c_str(),mess.c_str());
}


int main(int argc, char const *argv[])
{
    EventLoopThread thred;
    InetAddress serverAddr("127.0.0.1",9985);
    ChatClient client(thred.startLoop(),serverAddr);
    client.strat();
    std::string temp;
    while(std::getline(std::cin,temp))
    {
        client.send(temp);
    }
        
    return 0;
}


