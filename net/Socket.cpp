#include"Socket.h"
#include"InetAddress.h"
#include<stdio.h>
#include<assert.h>
#include<netinet/tcp.h>
#include<netinet/in.h>
const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
    return static_cast<const struct sockaddr*>(static_cast<const void*>(addr));
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in* addr)
{
    return static_cast<struct sockaddr*>(static_cast<void*>(addr));
}

const struct sockaddr_in* sockets::sockaddr_in_cast(const struct sockaddr* addr)
{
    return static_cast<const struct sockaddr_in*>(static_cast<const void *>(addr));

}

struct sockaddr_in* sockets::sockaddr_in_cast(struct sockaddr* addr)
{
    return static_cast<struct sockaddr_in*>(static_cast<void *>(addr));
}


int  sockets::createNonblocking(sa_family_t familt)
{
    int sockfd=::socket(familt,SOCK_CLOEXEC|SOCK_NONBLOCK|SOCK_STREAM,IPPROTO_TCP);
    // int sockfd=::socket(familt,SOCK_CLOEXEC|SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0)
    {
        perror("sockfd create error ");
    }
    return sockfd;
}

void sockets::Bind(int sockfd,const struct sockaddr* addr)
{
    socklen_t len=static_cast<socklen_t>(sizeof *addr);
    int ret=::bind(sockfd,addr,len);
    if(ret<0)
    {
        perror("sockfd bind error ");
    }
}

void sockets::Listen(int sockfd)
{
    int ret=::listen(sockfd,SOMAXCONN);
    if(ret<0)
    {
        perror("sockfd listen error ");
    }
}

int sockets::Accept(int sockfd,struct sockaddr* addr)
{
    socklen_t len=static_cast<socklen_t>(sizeof *addr);
    int connfd=accept4(sockfd,addr,&len,SOCK_NONBLOCK|SOCK_CLOEXEC);
    if(connfd<0)
    {
        int savedErrno=errno;
        perror("accept error ");
        switch (savedErrno)
        {
        case EAGAIN:
        case ECONNABORTED:
        case EINTR:
        case EPROTO: // ???
        case EPERM:
        case EMFILE: // per-process lmit of open file desctiptor ???
            // expected errors
            errno = savedErrno;
            break;
        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOBUFS:
        case ENOMEM:
        case ENOTSOCK:
        case EOPNOTSUPP:
            perror("accept error ");
            break;
        default:
            break;
        }
    
    }
    return connfd;
}

int sockets::Connet(int sockfd,const struct sockaddr* addr)
{
    socklen_t len=static_cast<socklen_t>(sizeof *addr);
    return ::connect(sockfd,addr,len);
}

void sockets::socket_close(int sockfd)
{
    if(::close(sockfd)<0)
    {
        perror("close sockfd error ");
    }
}

void sockets::shutdownWrite(int sockfd)
{
    if(::shutdown(sockfd,SHUT_WR)<0)
    {
        perror("shutWritedown error ");
    }
}

struct sockaddr_in sockets::getLocalAddr(int sockfd)
{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof addr);
    socklen_t len=static_cast<socklen_t>(sizeof addr);
    if(::getsockname(sockfd,sockaddr_cast(&addr),&len)<0)
    {
        perror("getLocalAddr error ");
    }
    return addr;
}

struct sockaddr_in sockets::getPeerAddr(int sockfd)
{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof addr);
    socklen_t len=static_cast<socklen_t>(sizeof addr);
    if(::getpeername(sockfd,sockaddr_cast(&addr),&len)<0)
    {
        perror("getPeerAddr error ");
    }
    return addr;
}

void sockets::toIp(char *buf,size_t size,const struct sockaddr* addr)
{
    assert(size>=INET_ADDRSTRLEN);
    const struct sockaddr_in* addr_=sockaddr_in_cast(addr);
    ::inet_ntop(AF_INET,&addr_->sin_addr,buf,static_cast<socklen_t>(size));
}

void sockets::toIpPort(char *buf,size_t size,const struct sockaddr* addr)
{
    toIp(buf,size,addr);
    const struct sockaddr_in* addr_=sockaddr_in_cast(addr);
    uint16_t port=networkToHost16(addr_->sin_port);
    int len=strlen(buf);
    assert(size>len);
    snprintf(buf+len,size-len,":%u",port);

}

int sockets::getSocketError(int sockfd)
{
  int optval;
  socklen_t optlen = static_cast<socklen_t>(sizeof optval);
  if(::getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&optval,&optlen)<0)
  {
      return errno;
  }
  else
  {
      return optval;
  }
  
}


Socket::~Socket()
{
    sockets::socket_close(sockfd_);
}

void Socket::bindAddress(const InetAddress &localaddr)
{
    sockets::Bind(sockfd_,localaddr.getSockAddr());
}

int Socket::accept(InetAddress* peeraddr)
{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof addr);
    struct sockaddr *addr_=sockets::sockaddr_cast(&addr);
    int connfd=sockets::Accept(sockfd_,addr_);
    if(connfd>=0)
    {
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite()
{
    sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNodelay(bool on)
{
    int optval=on ? 1 : 0;
    ::setsockopt(sockfd_,IPPROTO_TCP,TCP_NODELAY,&optval,static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
    int optval=on ? 1 : 0;
    ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&optval,static_cast<socklen_t>(sizeof optval));
}

void Socket::setKeepAlive(bool on)
{
    int optval=on ? 1 : 0;
    ::setsockopt(sockfd_,SOL_SOCKET,SO_KEEPALIVE,&optval,static_cast<socklen_t>(sizeof optval));

}

void Socket::listen()
{
    sockets::Listen(sockfd_);
}


