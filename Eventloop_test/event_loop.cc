#include"../EventLoop.h"
#include"../base/Logging.h"
void timeout();
EventLoop* loop_;
int main(int argc, char const *argv[])
{
    EventLoop loop;
    loop_=&loop;
    loop.runAt(3,timeout);
    loop.loop();
    return 0;
}
void timeout()
{
    printf("time out!!!!!!!!!!!!!!!!!!!");
    loop_->quit();
}
