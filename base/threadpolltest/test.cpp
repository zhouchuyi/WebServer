#include"../ThreadPoll.h"
#include<iostream>
#include<unistd.h>
#include<string.h>
void initthreadfun()
{
    char buf[32];
    snprintf(buf,sizeof buf,"begin in thread %5d\n",CurrentThread::tid());
    write(fileno(stdout),buf,strlen(buf));
}
void threadfun()
{
    char buf[32];
    snprintf(buf,sizeof buf,"now in thread %5d\n",CurrentThread::tid());
    write(fileno(stdout),buf,strlen(buf));
    sleep(1);
}
int main(int argc, char const *argv[])
{
    threadfun();
    ThreadPoll poll("test");
    poll.setMaxQueueSize(10);
    poll.setThreadInitCallback(initthreadfun);
    poll.start(5);
   while(true)
   {
       /* code */
       poll.run(threadfun);
   }
   
    
    

    return 0;
}
