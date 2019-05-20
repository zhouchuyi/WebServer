#include"InetAddress.h"


InetAddress::InetAddress(uint16_t port,bool loopbackOnly)
{
    memset(&addr_,0,sizeof addr_);
    addr_.sin_family=AF_INET;
    addr_.sin_addr.s_addr=sockets::hostToNetwork32(INADDR_ANY);
    addr_.sin_port=sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(const std::string ip,uint16_t port,bool loopbackOnly)
{
    memset(&addr_,0,sizeof addr_);
    addr_.sin_family=AF_INET;
    addr_.sin_port=sockets::hostToNetwork16(port);
    if(::inet_pton(AF_INET,ip.c_str(),&addr_.sin_addr)<=0)
    {
        perror("InetAddress: inet_pton error ");
    }
}

std::string InetAddress::toIp()const
{
    char buf[64]="";
    sockets::toIp(buf,sizeof buf,getSockAddr());
    return buf;
}

std::string InetAddress::toIpPort()const
{
    char buf[64]="";
    sockets::toIpPort(buf,sizeof buf,getSockAddr());
    return buf;
}





