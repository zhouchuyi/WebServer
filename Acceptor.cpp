#include"Acceptor.h"
#include"base/Logging.h"


Acceptor::Acceptor(EventLoop *loop,const InetAddress &listenAddr)
:   loop_(loop),
    acceptChannel_(loop,sockets::createNonblocking(AF_INET)),
    listenSocket_(acceptChannel_.fd()),
    listenning_(false),
    idleFd_(::open("/dev/null",O_RDONLY|O_CLOEXEC))
    {
     assert(idleFd_>=0);
     assert(listenSocket_.fd()>=0);
     listenSocket_.setReuseAddr(true);
    assert(acceptChannel_.fd()==listenSocket_.fd());
     listenSocket_.bindAddress(listenAddr);
     acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead,this));
    }

void Acceptor::listen()
{
	loop_->assertInLoopThread();
	listenning_=true;
    assert(listenSocket_.fd()>=0);
	listenSocket_.listen();
	acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
    loop_->assertInLoopThread();
    InetAddress peeraddress(0);
    Log<<"accepting in Acceptor::handleRead\n";
    int connfd=listenSocket_.accept(&peeraddress);
    if(connfd>=0)
    {
        //fix me
        
        if(newConnectionCallback_)
        {
            Log<<"accepted a new conn in Acceptor::handleRead\n";
            newConnectionCallback_(connfd,peeraddress);
        }
    }
    else
    {
        if(errno==EMFILE)
        {
            ::close(idleFd_),
            ::accept(listenSocket_.fd(),NULL,NULL);
            ::close(idleFd_);
            idleFd_=::open("/dev/null",O_RDONLY|O_CLOEXEC);
        }
        sockets::socket_close(connfd);
    }
    
}
Acceptor::~Acceptor()
{
    acceptChannel_.disableAll();
    acceptChannel_.remove();
    ::close(idleFd_);
}
