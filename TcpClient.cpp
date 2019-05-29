#include"TcpClient.h"
#include"EventLoop.h"
#include"net/Socket.h"
#include"base/Logging.h"
#include<functional>
TcpClient::TcpClient(EventLoop* loop,
                     const InetAddress& serverAddr,
                     std::string name)
 : loop_(loop),
   name_(name),
   retry_(false),
   connect_(false),
   nextConnId_(1),
   mutex_(),
   connector_(new Connector(loop_,serverAddr))
   {
       connector_->setNewConnectionCallback(std::bind(&TcpClient::newConnection,
                                                      this,
                                                      std::placeholders::_1));
        Log<<"TcpClient::TcpClient["<<name_<<"] \n";
   
   }


TcpClient::~TcpClient()
{
    Log<<"TcpClient::~TcpClient["<<name_<<"] \n";
}

void TcpClient::connect()
{
    Log<<"TcpClient::connect["<<name_<<"] - connecting to "<<connector_->serverAddress().toIpPort();
    connect_=true;
    connector_->start();
}
void TcpClient::disconnect()
{
    connect_=false;
    {
        MutexLockGuard lock(mutex_);
        if(connection_)
        {
            connection_->shutdown();
        }
    }
}
void TcpClient::stop()
{
    connect_=false;
    connector_->stop();
}

void TcpClient::newConnection(int sockfd)
{
    loop_->assertInLoopThread();
    InetAddress peer(sockets::getPeerAddr(sockfd));
    InetAddress local(sockets::getLocalAddr(sockfd));
    char buf[64];
    snprintf(buf,sizeof buf,"%s%u",peer.toIpPort().c_str(),nextConnId_);
    ++nextConnId_;
    std::string name=name_+buf;
    TcpConnectionPtr conn(new TcpConnection(loop_,name,sockfd,local,peer));
    conn->setConnectionCallback(connectioncallback_);
    conn->setMessageCallback(messagecallback_);
    conn->setCloseCallback(std::bind(&TcpClient::removeConnection,
                                    this,
                                 std::placeholders::_1));
    {
        MutexLockGuard lock(mutex_);
        conn_=conn;
    }
    conn->connectEstablished();
    // printf("success TcpClient::newConnection(int sockfd) \n");
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
    {
        MutexLockGuard lock(mutex_);
        assert(conn_==conn);
    }
    conn_.reset();
    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed,conn));
}
