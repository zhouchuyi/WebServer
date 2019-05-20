#include"../net/Socket.h"
#include"../net/InetAddress.h"
#include<iostream>
#define MAXLINE 512
int main(int argc, char const *argv[])
{
    // int sockfd,n;
    // char recvline[MAXLINE+1];
    // struct sockaddr_in servaddr;
    // if(argc!=2){
    //     fprintf(stderr,"argc error");
    //     exit(-1);
    // }
    // if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
    //     fprintf(stderr,"socket error");
    //     exit(-1);

    // }
    // __bzero(&servaddr,sizeof(servaddr));
    // servaddr.sin_family=AF_INET;
    // servaddr.sin_port=htons(9985);
    // if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0){
    //     fprintf(stderr,"inet_pton error");
    //     exit(-1);
        
    // }
    // if((connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)))<0){
    //     // fprintf(stderr,"connect error %d", s);
    //     perror("connect error: ");
    //     exit(-1);

    // }
    // while(n=read(sockfd,recvline,MAXLINE)>0){
    //     recvline[n]='\0';
    //     if(fputs(recvline,stdout)==EOF){
    //     fprintf(stderr,"write error");
    //     exit(-1);

    //     }
    //     /* code */
    // }
    // if(n<0){
    //     fprintf(stderr,"read error");
    //     exit(-1);

    // }

    Socket sockfd(sockets::createNonblocking(AF_INET));
    InetAddress address_("127.0.0.1",9985);
    sockets::Connet(sockfd.fd(),address_.getSockAddr());
    char buf[64];
    ssize_t n=sockets::read(sockfd.fd(),buf,sizeof buf);
    if(n<0)
        perror("read error");
    else
        std::cout<<n<<" "<<buf<<std::endl;
    return 0;
}
