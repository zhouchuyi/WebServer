#include"TcpServer.h"
#include"Acceptor.h"
#include"EventLoopThreadPool.h"
#include"base/Logging.h"
TcpServer::TcpServer(EventLoop* base,
              const InetAddress& listenAddr,
              const std::string& name,
              bool option)
 :loop_(base),
  name_(name),
  ipPort_(listenAddr.toIpPort()),
  acceptor_(new Acceptor(loop_,listenAddr)),
  threadPool_(new EventLoopThreadPool(loop_,name_)),
  connections_(),
  started_(false),
  nextConnId_(1)

{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection,this,
                                                    std::placeholders::_1,
                                                    std::placeholders::_2));

}

TcpServer::~TcpServer()
{
    loop_->assertInLoopThread();
    Log<<"TcpServer::~TcpServer() ["<<name_<<"] destroying\n";
    for (auto &item : connections_)
    {
        //for guard
        TcpConnectionPtr ptr(item.second);
        item.second.reset();
        //in ioloop destroy conn
        ptr->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed,ptr));
    }
    
}

void TcpServer::setThreadNum(int numThreads)
{
    assert(numThreads>=0);
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start()
{
    if(!started_)
    {
        started_=true;
        threadPool_->start(initCallback_);
    }
    assert(!acceptor_->isListening());
    loop_->runInLoop(std::bind(&Acceptor::listen,acceptor_.get()));

}

void TcpServer::newConnection(int sockfd,const InetAddress& peer)
{
    loop_->assertInLoopThread();
    EventLoop *ioloop=threadPool_->getNextLoop();
    char buf[64];
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    snprintf(buf,sizeof buf,"%s%u",ipPort_.c_str(),nextConnId_);
    ++nextConnId_;
    std::string name=name_+buf;
    Log<<"TcpServer::newConnection ["<<name_
        <<"] -new connection["<<name<<" ] from "<<peer.toIpPort()<<" \n";
    TcpConnectionPtr conn(new TcpConnection(ioloop,name,sockfd,localAddr,peer));
    conn->setConnectionCallback(connectioncallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection,
                                    this,
                                    std::placeholders::_1));
    //add setwritecompletecallback
    connections_[name]=conn;
    ioloop->runInLoop(std::bind(&TcpConnection::connectEstablished,conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    // if(loop_->isInLoopThread())
    // {
    //     removeConnectionInLoop(conn);
    // }
    // else
    // {
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop,this,conn));
    // }
    
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
    Log<<"TcpServer::removeConnectionInLoop ["<<conn->name()<<" ] \n";
    size_t n=connections_.erase(conn->name());
    assert(n==1);
    EventLoop *ioloop=conn->getLoop();
    ioloop->queueInLoop(std::bind(&TcpConnection::connectDestroyed,conn));
}