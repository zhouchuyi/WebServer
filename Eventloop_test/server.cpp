#include"../EventLoop.h"
#include"../Channel.h"
#include"../base/Logging.h"
#include"../Acceptor.h"
#include"../TcpConnection.h"
#include<iostream>
#include<sstream>
#include<fstream>

InetAddress localddress(9980);
void newconnection(int sockfd,const InetAddress& peeraddr);
void connectioncallback(const TcpConnectionPtr& conn);
void messagecallback(const TcpConnectionPtr& conn,Buffer* buf);
void closecallback(const TcpConnectionPtr& conn);
EventLoop* loop_=nullptr;
TcpConnectionPtr tie_;
std::string *file_=nullptr;
int main(int argc, char const *argv[])
{
    std::fstream in("A Final Reckoning.txt");
    std::string temp;
    std::ostringstream strm;
    while (std::getline(in,temp))
    {
        strm<<temp;
    }
    std::string file(std::move(strm.str()));
    file_=&file;
    EventLoop loop;
    loop_=&loop;
    Acceptor acceptor(&loop,localddress);
    acceptor.setNewConnectionCallback(newconnection);
    acceptor.listen();
    loop.loop();
    return 0;
}

void newconnection(int sockfd,const InetAddress& peeraddr)
{
    printf("set new Tcpconnection\n");
    TcpConnectionPtr conn(new TcpConnection(loop_,"test",sockfd,localddress,peeraddr));
    tie_=conn;
    conn->setConnectionCallback(connectioncallback);
    conn->setMessageCallback(messagecallback);
    conn->setCloseCallback(closecallback);
    loop_->runInLoop(std::bind(&TcpConnection::connectEstablished,conn));
    
}

void connectioncallback(const TcpConnectionPtr& conn)
{
    Log<<conn->localAddress().toIpPort()<<" -> "
       <<conn->peerAddress().toIpPort()<<" -- "
       <<(conn->connected() ? " ON ":" OFF ");
    // std::cout<<conn->localAddress().toIpPort()<<" -> "
    //    <<conn->peerAddress().toIpPort()<<" -- "
    //    <<(conn->connected() ? " ON ":" OFF ")<<std::endl;
    printf("%s",conn->peerAddress().toIpPort().c_str());
    conn->send(*file_);
}
void messagecallback(const TcpConnectionPtr& conn,Buffer* buf)
{
    const void* data=buf->peek();
    uint32_t be32=*static_cast<const uint32_t*>(data);
    int lwn=sockets::networkToHost32(be32);
    printf("send %d\n",lwn);
}
void closecallback(const TcpConnectionPtr& conn)
{
        Log<<conn->localAddress().toIpPort()<<" -> "
       <<conn->peerAddress().toIpPort()<<" -- "
       <<(conn->connected() ? " ON ":" OFF ");
    //     std::cout<<conn->localAddress().toIpPort()<<" -> "
    //    <<conn->peerAddress().toIpPort()<<" -- "
    //    <<(conn->connected() ? " ON ":" OFF ")<<std::endl;
       printf("close-------- %s",conn->peerAddress().toIpPort().c_str());
       tie_.reset();
       loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed,conn));
    
}
