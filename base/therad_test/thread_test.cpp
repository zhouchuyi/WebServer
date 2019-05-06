#include"../Thread.h"
#include"../mutex.h"
#include<functional>
#include<iostream>
static int num=5;
MutexLock mutex;
void Thread_print(){
    MutexLockGuard lock(mutex);
    std::cout<<CurrentThread::tid()<<": "<<++num<<std::endl;
}
int main(int argc, char const *argv[])
{
    // Thread::ThreadFunc func(Thread_print);
    // Thread thread(func);
    // thread.start();
    // {
    //     MutexLockGuard lock(mutex);
    //     std::cout<<CurrentThread::tid()<<": "<<++num<<std::endl;
    // }
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME,&abstime);
    time_t now=abstime.tv_sec;
    Condition cond(mutex);
    cond.waitForSeconds(3);
    clock_gettime(CLOCK_REALTIME,&abstime);
    std::cout<<abstime.tv_sec-now<<std::endl;
    return 0;
}

