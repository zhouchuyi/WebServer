#include"EventLoop.h"
#include"poll.h"
#include"TimeQueue.h"


__thread EventLoop* t_loopInthisThread=nullptr;

// EventLoop::EventLoop()
//  : threadId_(CurrentThread::tid()),
//     looping_(false),
//     quit_(false),
//     eventHandling_(false),
//     callingPendingFunctors_(false),
//     iteration(0),
//     pollReturnTime(0),
//     ,