#ifndef NET_ADDRESS_H_
#define NET_ADDRESS_H_

#include<string>
#include<netinet/in.h>
#include"Socket.h"

class InetAddress
{
public:
    // for server only set port
    explicit InetAddress(uint16_t port,bool loopbackOnly=false);
    //for client,need to set ip and port
    explicit InetAddress(const struct sockaddr_in& addr)
     :addr_(addr)
     { }
    explicit InetAddress(const std::string ip,uint16_t port,bool loopbackOnly=false);
    InetAddress()=default;
    ~InetAddress()=default;

    sa_family_t familt() const
    {
        return addr_.sin_family;
    }
    std::string toIp()const;
    std::string toIpPort()const;
    uint16_t toPort()const
    {
        return addr_.sin_port;
    }

    const struct sockaddr* getSockAddr()const
    {
        return sockets::sockaddr_cast(&addr_);
    } 

    void setSockAddr(const struct sockaddr_in & addr)
    {
        addr_=addr;
    }
    
private:
    struct sockaddr_in addr_;
};



#endif