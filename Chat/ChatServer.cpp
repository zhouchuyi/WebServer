#include"codec.h"
#include"../TcpServer.h"
#include"../base/ThreadLocalSingleton.h"
#include"../EventLoop.h"
#include<set>

using namespace std::placeholders; 

class ChatServer:noncopyable
{
public:
    ChatServer(EventLoop* loop,const InetAddress& listenAddr)
     : server_(loop,listenAddr,"ChatServer"),
       codec_(std::bind(&ChatServer::onStringMessage,this,_1,_2)),
       mutex_()
     { 
        server_.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
        server_.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage,&codec_,_1,_2));
        server_.setThreadInitCallback(std::bind(&ChatServer::ThreadInit,this,_1));
     }
    ~ChatServer()=default;
    
    void start()
    { server_.start(); }
    void setThreadNum(int n)
    { server_.setThreadNum(n); }

private:

    typedef std::set<TcpConnectionPtr> Connectionlist;
    typedef ThreadLocalSingleton<Connectionlist> LocalConnectionList;

    void onConnection(const TcpConnectionPtr&);
    void onStringMessage(const TcpConnectionPtr&,const std::string&);
    
    void distributeMessage(const std::string& message);
    void ThreadInit(EventLoop* loop);

    TcpServer server_;
    LengthHeaderCodec codec_;
    MutexLock mutex_;
    std::set<EventLoop*> loops_;

};


void ChatServer::onConnection(const TcpConnectionPtr& conn)
{
    Log<<conn->peerAddress().toIpPort()<<" -> "
       <<conn->localAddress().toIpPort()
       <<(conn->connected() ? " ON " : " OFF ");
    if(conn->connected())
    {
        auto res=LocalConnectionList::instace().insert(conn);
        assert(res.second==true);
        char buf[64];
        snprintf(buf,sizeof buf,"Welcome --%s--!!",conn->peerAddress().toIpPort().c_str());
        std::string message(buf);
        EventLoop::Functor f=std::bind(&ChatServer::distributeMessage,this,message);
        std::set<EventLoop*> temp;
        {
            MutexLockGuard lcok(mutex_);
            temp=loops_;
        }
        for (auto &ioloop : temp)
        {
            ioloop->queueInLoop(f);
        }
    }
    else
    {
        auto res=LocalConnectionList::instace().erase(conn);
        assert(res==true);
        char buf[64];
        snprintf(buf,sizeof buf,"GoodBye --%s--!!",conn->peerAddress().toIpPort().c_str());
        std::string message(buf);
        EventLoop::Functor f=std::bind(&ChatServer::distributeMessage,this,message);
        std::set<EventLoop*> temp;
        {
            MutexLockGuard lcok(mutex_);
            temp=loops_;
        }
        for (auto &ioloop : temp)
        {
            ioloop->queueInLoop(f);
        }
    }
    
}
void ChatServer::onStringMessage(const TcpConnectionPtr& conn,const std::string& message)
{
    // printf("%s",message.c_str());
    EventLoop::Functor f=std::bind(&ChatServer::distributeMessage,this,message);
    std::set<EventLoop*> temp;
    {
        MutexLockGuard lcok(mutex_);
        temp=loops_;
    }
    for (auto &ioloop : temp)
    {
        ioloop->queueInLoop(f);
    }
    


}

void ChatServer::distributeMessage(const std::string& message)
{
    for (auto &conn : LocalConnectionList::instace())
    {
        codec_.send(conn,message);
    }
    
}
void ChatServer::ThreadInit(EventLoop* loop)
{
    assert(LocalConnectionList::pointer()==NULL);
    LocalConnectionList::instace();
    assert(LocalConnectionList::pointer()!=NULL);
    MutexLockGuard lock(mutex_);
    auto res=loops_.insert(loop);
    assert(res.second==true);
}

int main(int argc, char const *argv[])
{
    EventLoop base;
    InetAddress listenAddr(9985);
    ChatServer server(&base,listenAddr);
    server.setThreadNum(3);
    server.start();
    base.loop();
    return 0;
}
