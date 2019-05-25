
#include"Connector.h"
#include"EventLoop.h"
#include"net/Socket.h"
#include"base/Logging.h"
#include"Channel.h"
#include<assert.h>

static const int kInitRetryDelayMs=500;
static const int kMaxRetryDelayMs=30000;

Connector::Connector(EventLoop* loop,const InetAddress& serverAddr)
 : loop_(loop),
   serverAddr_(serverAddr),
   state_(kDisconnected),
   connect_(false),
   retryDelayMs_(kInitRetryDelayMs)
{

}

Connector::~Connector()
{
    assert(!channel_);
}

void Connector::start()
{
    connect_=true;
    loop_->runInLoop(std::bind(&Connector::startInLoop,this));
}



void Connector::startInLoop()
{
    loop_->assertInLoopThread();
    assert(state_==kDisconnected);
    if(connect_)
    {
        connect();
    }
    else
    {
        Log<<" not connect ";
    }
    

}



//start to connect
void Connector::connect()
{
    int sockfd=sockets::createNonblocking(AF_INET);
    int ret=sockets::Connet(sockfd,serverAddr_.getSockAddr());
    int savedErrno=(ret==0)?0:errno;
    switch (savedErrno)
    {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
        printf("connecting\n");
        connecting(sockfd);
        break;
    
    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
        printf("case retry\n");
        retry(sockfd);
        break;
    
    default:
        Log << "connect error in Connector::startInLoop " << savedErrno;
        ::close(sockfd);
        break;
    }
}

void Connector::connecting(int sockfd)
{
    setState(kConnecting);
    assert(!channel_);
    //set channel
    channel_.reset(new Channel(loop_,sockfd));
    assert(channel_->fd()==sockfd);
    channel_->setWriteCallback(std::bind(&Connector::handleWrite,this));
    channel_->setErrorCallback(std::bind(&Connector::handleError,this));
    channel_->enableWritinging();
}
void Connector::handleWrite()
{
    Log<<" Connector::handleWrite() \n";
    if(state_==kConnecting)
    {
        //remove channel no matter error or success
        int sockfd=resetChannel();
        int err=sockets::getSocketError(sockfd);
        if(err)
        {
            Log<<" Connector::handleWrite() -SO_ERROR "<<err<<"\n";
            retry(sockfd);
        }
        else
        {
            setState(kConnected);
            if(connect_)
            {
                Log<<"Connector::handleWrite connectioncallback-------------------\n";
                connectioncallback_(sockfd);
            }
            else
            {
                ::close(sockfd);
            }
            
        }
        
    }
    else
    {
        assert(state_==kDisconnected);
    }
    
}

void Connector::handleError()
{
    printf(" Connector::handleError() \n");
    Log<<" Connector::handleError() "<<state_;
    if(state_==kConnecting)
    {
        int sockfd=resetChannel();
        int err=sockets::getSocketError(sockfd);
        Log<<" SO_ERROR = "<<err;
        retry(sockfd);
    }
}

void Connector::retry(int sockfd)
{
    //close sockfd
    sockets::socket_close(sockfd);
    setState(kDisconnected);
    if(connect_)
    {
        printf("Connector::retry\n");
        Log<<" Connector::retry in "<<serverAddr_.toIpPort()<<" "<<retryDelayMs_<<" milliseconds. ";
        loop_->runAfter(retryDelayMs_/1000,std::bind(&Connector::startInLoop,this));
        retryDelayMs_=std::min(retryDelayMs_*2,kMaxRetryDelayMs);
    }

}
int Connector::resetChannel()
{
    // remove channel
    channel_->disableAll();
    channel_->remove();
    int sockfd=channel_->fd();
    loop_->queueInLoop(std::bind(&Connector::resetChannelInLoop,this));
    return sockfd;
}

void Connector::resetChannelInLoop()
{
    loop_->assertInLoopThread();
    channel_.reset();
}



void Connector::stop()
{
    connect_=false;
    loop_->runInLoop(std::bind(&Connector::stopInLoop,this));
}
void Connector::stopInLoop()
{
    loop_->assertInLoopThread();
    if(state_==kConnecting)
    {
        setState(kDisconnected);
        int sockfd=resetChannel();
        retry(sockfd);
    }
}
