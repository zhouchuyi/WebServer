#include"../net/Socket.h"
#include"../net/InetAddress.h"
#include"../Acceptor.h"
#include<iostream>


int main(int argc, char const *argv[])
{

    
    int sockfd=::socket(AF_INET,SOCK_CLOEXEC|SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0)
    {
        perror("sockfd create error ");
    }
    InetAddress address_("127.0.0.1",9980);
    int ret;
again1: ret=sockets::Connet(sockfd,address_.getSockAddr());
        if(ret<0)
            goto again1;
    std::cout<<"success\n";
    char buf[64];
again2: ssize_t n=sockets::read(sockfd,buf,sizeof buf);
        if(n<0)
        {
            perror("read error ");
            goto again2;
        }
            
    else
        std::cout<<buf<<std::endl;
    return 0;
}


