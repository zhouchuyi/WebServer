#include"../EventLoop.h"
#include"../net/InetAddress.h"
#include"../Connector.h"
#include"../Buffer.h"
#include"../TcpClient.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<fcntl.h>
EventLoop* glob_loop=nullptr;
void connectioncallback(const TcpConnectionPtr&);
void messagecallabck(const TcpConnectionPtr&,Buffer*);
int main(int argc, char const *argv[])
{
    // Buffer buf;
    // // int sockfd=::socket(AF_INET,SOCK_CLOEXEC|SOCK_STREAM,IPPROTO_TCP);
    // if(sockfd<0)
    // {
    //     perror("sockfd create error ");
    // }
    InetAddress address_("127.0.0.1",9985);
    EventLoop loop;
    glob_loop=&loop;
    TcpClient client(&loop,address_,"test");
    client.setConnectionCallback(connectioncallback);
    client.setMessageCallback(messagecallabck);
    client.connect();
    
    loop.loop();
    // int ret;
    // ret=sockets::Connet(sockfd,address_.getSockAddr());
    // if(ret<0)
    //     std::cout<<"fail\n";
    // else
    //     std::cout<<"success\n";
    // ssize_t n;
    // int err;
    // n=buf.readFd(sockfd,&err);
    // printf("received %d\n",n);
    // printf("%s\n",buf.retrieveAsString(n).c_str());
    // std::cout<<"input :";
    // std::string message;
    // std::cin>>message;
    // n=::write(sockfd,message.c_str(),message.size());
    // uint32_t len=sockets::hostToNetwork32(static_cast<uint32_t>(n));
    // ::write(sockfd,&len,sizeof len);
    // // printf("%s",buf.retrieveAsString(n).c_str());
    
    
        
    return 0;
}

// void conncallback(int sockfd)
// {
//     Buffer buf;
//     int err;
//     ssize_t n;
//     while((n=buf.readFd(sockfd,&err))>0)
//     {
//         printf("%s\n",buf.retrieveAsString(n).c_str());
//     }
//     printf("received %d\n",n);
    
//     ::close(sockfd);
//     glob_loop->quit();
// }

void connectioncallback(const TcpConnectionPtr& conn)
{
    printf("success connected to %s\n",conn->peerAddress().toIpPort().c_str());
    if(conn->connected())
    {
        Buffer buf;
        char str[64];
        snprintf(str,sizeof buf,"hello from %s\n",conn->localAddress().toIpPort().c_str());
        std::string mess(str);
        printf("string::--%s--%d\n",mess.c_str(),mess.size());
        buf.append(mess);
        uint32_t len=static_cast<uint32_t>(mess.size());
        printf("%d\n",len);
        uint32_t be32=sockets::hostToNetwork32(len);
        printf("%d",be32);
        buf.prepend(&be32,sizeof be32);
        conn->send(&buf);
        // conn->send("hello\n",6);
        // char buf[64];
        // snprintf(buf,sizeof buf,"hello from %s\n",conn->localAddress().toIpPort().c_str());
        // conn->send(buf);
    }
    else
    {
        
        printf("begin to send bye\n");
    }
    

}
void messagecallabck(const TcpConnectionPtr& conn,Buffer* buf)
{
    printf("received message \n");
    conn->shutdown();
}
