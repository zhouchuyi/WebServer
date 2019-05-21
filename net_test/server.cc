#include"../net/Socket.h"
#include"../net/InetAddress.h"
#include"../Acceptor.h"
#include<iostream>

void newconnection(int sockfd,const InetAddress& peerAddr);


int main(int argc, char const *argv[])
{
    

    // Socket sockfd(sockets::createNonblocking(AF_INET));
    // sockfd.bindAddress(address_);
    // sockfd.listen();
    // const char buf[]="hello\n";
    // for(;;)
    // {
    //     InetAddress addr;
    //     Socket connfd(sockfd.accept(&addr));
    //     std::cout<<addr.toIpPort()<<std::endl;
    //     sockets::write(connfd.fd(),buf,sizeof buf);
    //     connfd.shutdownWrite();
    // }
    EventLoop loop;    
    InetAddress address_(9985);
    Acceptor acceptor(&loop,address_);
    acceptor.setNewConnectionCallback(&newconnection);
    acceptor.listen();
    loop.loop();
    return 0;
}

void newconnection(int sockfd,const InetAddress& peerAddr)
{
    printf("newconnection(): accept a new connection from %s\n",peerAddr.toIpPort().c_str());
    ::write(sockfd,"how are you?\n",13);
    sockets::socket_close(sockfd);
}