#include"../net/Socket.h"
#include"../net/InetAddress.h"
#include"../Acceptor.h"
#include"../Buffer.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<fcntl.h>
int main(int argc, char const *argv[])
{
    Buffer buf;
    int sockfd=::socket(AF_INET,SOCK_CLOEXEC|SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0)
    {
        perror("sockfd create error ");
    }
    InetAddress address_("127.0.0.1",9980);
    int ret;
    ret=sockets::Connet(sockfd,address_.getSockAddr());
    if(ret<0)
        std::cout<<"fail\n";
    else
        std::cout<<"success\n";
    ssize_t n;
    int err;
    n=buf.readFd(sockfd,&err);
    printf("received %d",n);
    uint32_t len=sockets::hostToNetwork32(static_cast<uint32_t>(n));
    ::write(sockfd,&len,sizeof len);
    // printf("%s",buf.retrieveAsString(n).c_str());
    ::close(sockfd);
    
        
    return 0;
}


