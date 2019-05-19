#ifndef NET_ADDRESS_H_
#define NET_ADDRESS_H_

#include<string>
#include<netinet/in.h>

class InetAddress
{
public:
    explicit InetAddress(uint16_t port,bool );
    ~InetAddress();
private:
    struct sockaddr_in addr_;
};



#endif