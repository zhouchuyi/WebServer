#include"TcpConnection.h"
#include"base/Logging.h"
#include"Channel.h"
#include"EventLoop.h"
#include<utility>
void defaultConnectionCallback(const TcpConnectionPtr& conn)
{
    Log<<conn->localAddress().toIpPort()<<" -> "
       <<conn->peerAddress().toIpPort()<<" -- "
       <<(conn->connected() ? " ON ":" OFF ");
}

void defaultMessageCallback(const TcpConnectionPtr& conn,Buffer* buf)
{
    buf->retriveAll();
}


TcpConnection::TcpConnection(EventLoop* loop,
                             const std::string name,
                             int sockfd,
                             const InetAddress& local,
                             const InetAddress& peer)
  : loop_(loop),
    name_(name),
    socket_(new Socket(sockfd)),
    reading_(false),
    state_(kConnecting),
    localAddr_(local),
    peerAddr_(peer),
    channel_(new Channel(loop,sockfd)),
    highWaterMark_(64*1024*1024)
{
    assert(channel_->fd()==socket_->fd());
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead,this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite,this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose,this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError,this));
    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    //add log info
    printf("TcpConnection::~TcpConnection()\n");
    Log<<"TcpConnection::~TcpConnection ["<<name_<<" ] \n";
    assert(state_==kDisconnected);
}


void TcpConnection::send(const std::string& message)
{
    if(state_==kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(message.c_str(),message.size());
        }
        else
        {
            loop_->runInLoop(std::bind(&TcpConnection::sendInLoop,this,message.c_str(),message.size()));
        }
        

    }
}
void TcpConnection::send(const void* message,size_t len)
{
    if(state_==kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(static_cast<const char*>(message),len);
        }
        else
        {
            loop_->runInLoop(std::bind(&TcpConnection::sendInLoop,this,static_cast<const char*>(message),len));
        }
        

    }
}
void TcpConnection::send(Buffer* buf)
{
    if(state_==kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(buf->peek(),buf->readableBytes());
            buf->retriveAll();
        }
        else
        {
            loop_->runInLoop(std::bind(&TcpConnection::sendInLoop,this,buf->peek(),buf->readableBytes()));
            buf->retriveAll();
        }
        
    }
}

void TcpConnection::sendInLoop(const char* message,size_t len)
{
    loop_->assertInLoopThread();
    ssize_t nwrote=0;
    if(state_==kDisconnected)
    {
        printf("disconnected, give up send\n");
        Log<<"disconnected, give up send ";
        return;
    }
    if(!channel_->isWriting()&&outputBuffer_.readableBytes()==0)
    {
        Log<<"begin to write \n";
        nwrote=::write(channel_->fd(),message,len);
        if(nwrote>=0&&nwrote<len)
            Log<<"write more data on "<<peerAddr_.toIpPort();
        if(nwrote<0)
        {
            Log<<"error in sendInLoop "<<peerAddr_.toIpPort();
            perror("error in sendInLoop ");
            nwrote=0;
        }
    }
    assert(nwrote>=0);
    if(static_cast<size_t>(nwrote) < len)
    {
        outputBuffer_.append(message+nwrote,len-nwrote);
        if(!channel_->isWriting())
        {
            channel_->enableWritinging();
        }
    }


}


void TcpConnection::handleRead()
{
    loop_->assertInLoopThread();
    int savedErrno=0;
    ssize_t n=inputBuffer_.readFd(channel_->fd(),&savedErrno);
    if(n>0)
    {
        messageCallback_(shared_from_this(),&inputBuffer_);
    }
    else if (n==0)
    {
        printf("begin in handle close\n");
        handleClose();
    }
    else
    {
        errno=savedErrno;
        Log<<"error in handleRead "<<peerAddr_.toIpPort();
        handleError();
    }
    
}

void TcpConnection::handleWrite()
{
    Log<<" TcpConnection::handleWrite \n";
    loop_->assertInLoopThread();
    if(channel_->isWriting())
    {
        ssize_t n=::write(channel_->fd(),outputBuffer_.peek(),outputBuffer_.readableBytes());
        if(n>0)
        {
            outputBuffer_.retrieve(n);
            //add writecompletecall
            if(outputBuffer_.readableBytes()==0)
            {
                channel_->disableWriting();
                if(state_==kDisconnecting)
                {
                    shutdownInLoop();
                }
            }
            else
            {
                Log<<"write more data on "<<peerAddr_.toIpPort();
            }
    
        }
        else
        {
            Log<<"error in handlewrite "<<peerAddr_.toIpPort();
            perror("error in handlewrite ");
        }
    }
    else
    {
        Log<<"write down,no more writting ";
    }
       
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    assert(state_==kConnected||state_==kDisconnecting);
    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());
    closeCallback_(shared_from_this());

}

void TcpConnection::shutdown()
{
    if(state_==kConnected)
    {
        setState(kDisconnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop,this));
    }
}

void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    if(!channel_->isWriting())
        socket_->shutdownWrite();
}


void TcpConnection::setTcpNodelay(bool on)
{
    socket_->setTcpNodelay(on);
}

void  TcpConnection::startRead()
{
    loop_->runInLoop(std::bind(&TcpConnection::startReadInLoop,this));
}
void TcpConnection::stopread()
{
    loop_->runInLoop(std::bind(&TcpConnection::stopReadInLoop,this));
}
void TcpConnection::startReadInLoop()
{
    loop_->assertInLoopThread();
    channel_->enableReading();
    reading_=true;
    
}

void TcpConnection::stopReadInLoop()
{
    loop_->assertInLoopThread();
    channel_->disableReading();
    reading_=false;
}

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    assert(state_==kConnecting);
    setState(kConnected);
    assert(state_==kConnected);
    channel_->tie(shared_from_this());
    channel_->enableReading();
    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
    loop_->assertInLoopThread();
    setState(kDisconnected);
    channel_->disableAll();
    channel_->remove();
}
void TcpConnection::handleError()
{
    // char buf[512];
    int savederrno=sockets::getSocketError(channel_->fd());
    Log<<"Tcpconnection handleError "<<name_<<" SO_ERROR="<<savederrno<<"\n";
}
const char* TcpConnection::stateToString()const
{
    switch (state_)
    {
    case kConnected:
        return "kConnected";
    case kDisconnected:
        return "kDisconnected";
    case kDisconnecting:
        return "kDisconnecting";
    case kConnecting:
        return "kConnecting";
    default:
        return "unkonwn state";
    }
}
