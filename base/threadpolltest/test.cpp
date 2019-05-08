#include"../ThreadPoll.h"
#include<iostream>
static int num=0;
void Initthreadfun()
{
    std::cout<<
}
int main(int argc, char const *argv[])
{
    ThreadPoll poll("test");
    poll.setMaxQueueSize(10);
    poll.setThreadInitCallback();
    return 0;
}
