#include"../EventLoop.h"
#include"../Channel.h"
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/timerfd.h>
#include"../base/Logging.h"
EventLoop *g_loop;
Channel*  g_channel;
const char *str="time out/n";
void timeout()
{
    // Log<<"time out\n";
    write(fileno(stdout),str,strlen(str));
}
int main(int argc, char const *argv[])
{
    EventLoop loop;
    g_loop=&loop;

    // int timerfd=timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC|TFD_NONBLOCK);

    // Channel channel(&loop,timerfd);
    // g_channel=&channel;
    // channel.setReadCallback(timeout);
    // channel.enableReading();
    g_loop->runEvery(3.0,timeout);
    loop.loop();
    
}
