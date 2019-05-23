#include"../EventLoopThreadPool.h"
#include"../base/Logging.h"
#include"../TcpServer.h"
void connectioncallback(const TcpConnectionPtr&);
void messagecallback(const TcpConnectionPtr&,Buffer*);
int main(int argc, char const *argv[])
{
    EventLoop base;
    InetAddress listenaddr_(9985);
    // EventLoopThreadPool pool(&base,"test");
    // pool.setThreadNum(3);
    // pool.start(init);
    // base.loop();
    TcpServer server(&base,listenaddr_,"test");
    server.setThreadNum(3);
    server.setConnectionCallback(connectioncallback);
    server.setMessageCallback(messagecallback);
    server.start();
    base.loop();
    return 0;
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
    char buf[128];
    snprintf(buf,sizeof buf,"SUCCESS___IN__THREAD %d",CurrentThread::tid());
    conn->send(buf,strlen(buf));
}
void messagecallback(const TcpConnectionPtr& conn,Buffer* buf)
{
    size_t n=buf->readableBytes();
    std::string mess=buf->retrieveAsString(n);
    printf("%s\n",mess.c_str());
}

// void init(EventLoop *loop)
// {
//     loop->runEvery(2,timeout);
// }

// void timeout()
// {
//     Log<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!time out!!!!!!"<<CurrentThread::tid()<<"\n";
// }