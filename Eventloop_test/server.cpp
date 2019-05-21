#include"../EventLoop.h"
#include"../Channel.h"
#include"../base/Logging.h"
#include"../Acceptor.h"
#include<iostream>
void timeout();
void newconnection(int sockfd,const InetAddress& addr);
// Socket sockfd(sockets::createNonblocking(AF_INET));
void call();
int main(int argc, char const *argv[])
{
    
    
    // // const char buf[]="hello\n";
    // // for(;;)
    // // {
    // //     InetAddress addr;
    // //     Socket connfd(sockfd.accept(&addr));
    // //     std::cout<<addr.toIpPort()<<std::endl;
    // //     sockets::write(connfd.fd(),buf,sizeof buf);
    // //     connfd.shutdownWrite();
    // // }

    // InetAddress address_(9981);
    // sockfd.bindAddress(address_);
    // sockfd.listen();
    // EventLoop loop;
    // Channel channel_(&loop,sockfd.fd());
    // channel_.setReadCallback(call);
    // channel_.enableReading();
    // loop.loop();
    EventLoop loop;
    InetAddress address_(9980);
    Acceptor acceptor(&loop,address_);
    acceptor.setNewConnectionCallback(newconnection);
    acceptor.listen();
    loop.loop();
    // EventLoop loop;
    // loop.runEvery(3,timeout);
    // loop.loop();
    return 0;
}

void timeout()
{
    Log<<"time out\n";
}
void newconnection(int sockfd,const InetAddress& addr)
{
    const char *buf="hello\n";
    ::write(sockfd,buf,sizeof buf);
}
// void call()
// {
//     printf("accepting......\n");
//     InetAddress peer;
//     sockfd.accept(&peer);
//     printf("done\n");
// }