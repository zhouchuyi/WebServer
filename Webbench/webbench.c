#include<time.h>
#include<signal.h>
#include<unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include<sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
volatile int timerexpired=0;
int speed=0;
int failed=0;
int bytes=0;

int client=1;
int benchtime=30;
int mypipe[2];
int bench(const char* ip,int port);
void benchcore(const char* ip,int port);
int Socket(const char* ip,int port);

void alarm_handler(int signal)
{
    timerexpired=1;
}
int main(int argc, char const *argv[])
{
    if(argc!=5)
    {
        printf("error in argc\n");
        exit(0);
    }
    const char* ip=argv[1];
    int port=atoi(argv[2]);
    client=atoi(argv[3]);
    benchtime=atoi(argv[4]);

    printf("Running info: ");

    if(client==1)
        printf("1 client");
    else
        printf("%d clients",client);
    printf(", running %d sec\n",benchtime);
    return bench(ip,port);
}
int bench(const char* ip,int port)
{
    int i,j,k;
    pid_t pid;
    i=Socket(ip,port);
    FILE* f;
    if(i<0)
    {
        printf("\nConnect to server failed. Aborting benchmark.\n");
        return 1;
    }
    close(i);

    if(pipe(mypipe)<0)
    {
        perror("pipe failed.");
        return 1;
    }
    for (i = 0; i < client; i++)
    {
        pid=fork();
        if( pid <= (pid_t) 0)
        {
            sleep(1);
            break;
        }
    }
    if(pid < (pid_t)0 )
    {
        perror("fork error.");
        return 1;
    }
    if(pid == (pid_t)0)
    {
        //in child
        benchcore(ip,port);
        f=fdopen(mypipe[1],"w");
        if(f==NULL)
        {
            perror("open pipe for writing failed.");
            return 1;
        }
        fprintf(f,"%d %d %d\n",speed,failed,bytes);
        fclose(f);
        return 0;
    }
    else
    {
        //in parents
        f=fdopen(mypipe[0],"r");
        if(f==NULL)
        {
            perror("open pipe for writing failed.");
            return 1;
        }
        speed=0;
        failed=0;
        bytes=0;
        // while (1)
        // {
        //     printf("begin in canf\n");
        //     pid=fscanf(f,"%d %d %d",&i,&j,&k);
        //     if(pid<2)
        //     {
        //         printf("some children died.\n");
        //         break;
        //     }
        //     speed+=i;
        //     failed+=j;
        //     bytes+=k;
        //     if(--client==0)
        //     {
        //         break;
        //     }
        // }
        for (int t = 0; t < client; t++)
        {
            printf("begin in canf\n");
            pid=fscanf(f,"%d %d %d",&i,&j,&k);
            if(pid<2)
            {
                printf("some children died.\n");
                break;
            }
            speed+=i;
            failed+=j;
            bytes+=k;
            
        }
        
        fclose(f);
        printf("\nSpeed=%d times/min, %d bytes/sec.\nRequests: %d susceed, %d failed.\n",
            (int)((speed+failed)/(benchtime/60.0f)),
            (int)(bytes/(float)benchtime),
            speed,
            failed);
        
    }
    return 1;
    
}

void benchcore(const char* ip,int port)
{
    int rlen;
    char buf[1500];
    int s,i;
    signal(SIGALRM,alarm_handler);
    alarm(benchtime);
    nexttry:while (1)
    {
        if(timerexpired)
        {
            if(failed>0)
                failed--;
            return;
        }
        
        s=Socket(ip,port);
        if(s<0){ failed++;continue;}
        if(shutdown(s,SHUT_WR)<0){failed++;continue;}
        while (1)
        {
            if(timerexpired) break;
            i=read(s,buf,sizeof buf);
            if(i<0)
            {
                failed++;
                close(s);
                goto nexttry;
            }
            else
            if(i==0) break;
            else
            bytes+=i;
            
        }
        if(close(s)<0) {failed++;continue;}
        speed++;
        
    }
    
    
}
int Socket(const char* ip,int port)
{
    int sock;
    struct sockaddr_in addr;
    __bzero(&addr,sizeof addr);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    if(inet_pton(AF_INET,ip,&addr.sin_addr)<0)
    {
        return -1;
    }
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
        return sock;
    if(connect(sock,(struct sockaddr*)&addr,sizeof addr)<0)
    {
        return -1;
    }
    return sock;
}
