#include"../net/Socket.h"
#include"../net/InetAddress.h"
#include<iostream>
#define MAXLINE 512
int main(int argc, char const *argv[])
{
    // int listedfd,connfd;
    // int i,maxi,maxfd,sockfd;
    // int nready,client[FD_SETSIZE];
    // ssize_t n;
    // fd_set rset,allset;
    // socklen_t len;
    // struct sockaddr_in seraddr,cliaddr;
    // char buf[MAXLINE+1];
    // time_t ticks;

    // if((listedfd=socket(AF_INET,SOCK_STREAM,0))<0)
    // {
    //     perror("lister error ");
    //     exit(-1);
    // }
    // __bzero(&seraddr,sizeof(seraddr));
    // seraddr.sin_family=AF_INET;
    // seraddr.sin_port=htons(9985);
    // seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    // if(bind(listedfd,(struct sockaddr*)&seraddr,sizeof(seraddr))<0){
    //     fprintf(stderr,"bind error");
    //     exit(-1);
    // }
    // char *ptr;
    // int backlog;
    // if((ptr=getenv("LISTENQ"))!=NULL){
    //     backlog=atoi(ptr);
    // }
    // if(listen(listedfd,backlog)<0){
    //     fprintf(stderr,"listn error");
    // }

    // maxfd=listedfd;
    // maxi=-1;
    // for(i = 0; i < FD_SETSIZE; i++)
    // {
    //     client[i]=-1;
    // }
    // FD_ZERO(&allset);
    // FD_ZERO(&rset);
    // for(;;)
    // {
    //     rset=allset;
    //     nready=select(maxfd+1,&rset,NULL,NULL,NULL);
    //     if(FD_ISSET(listedfd,&rset)){
            
    //     }
    // }
    // for(;;)
    // {
    //     len=sizeof(cliaddr);
    //     if((connfd=accept(listedfd,NULL,NULL))<0){
    //         perror("accept error: ");
    //         exit(-1);
    //     }
    //     // printf("connection from %s,port %d\n",
    //     // inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)),
    //     // ntohs(cliaddr.sin_port));

    //     ticks=time(NULL);
    //     snprintf(buf,sizeof(buf),"%.24s\r\n",ctime(&ticks));
    //     write(connfd,buf,sizeof(buf));
    //     close(connfd);
    // }
    Socket sockfd(sockets::createNonblocking(AF_INET));
    InetAddress address_(9985);
    sockfd.bindAddress(address_);
    sockfd.listen();
    const char buf[]="hello\n";
    for(;;)
    {
        InetAddress addr;
        Socket connfd(sockfd.accept(&addr));
        std::cout<<addr.toIpPort()<<std::endl;
        sockets::write(connfd.fd(),buf,sizeof buf);
        connfd.shutdownWrite();
    }
    
    
    
    return 0;
}
