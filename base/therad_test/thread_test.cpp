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
    Thread::ThreadFunc func(Thread_print);
    Thread thread(func);
    thread.start();
    {
        MutexLockGuard lock(mutex);
        std::cout<<CurrentThread::tid()<<": "<<++num<<std::endl;
    }
    return 0;
}

