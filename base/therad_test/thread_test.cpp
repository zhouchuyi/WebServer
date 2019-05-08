#include"../Thread.h"
#include"../mutex.h"
#include"../BlockingQueue.h"
#include<functional>
#include<iostream>
#include<string>
// static int num=5;
// MutexLock mutex;
BlockingQueue<std::string> queue;
void Thread_print(){
    std::string temp;
    while(true){
        temp=queue.take();
        std::cout<<temp<<std::endl;
        if(temp=="stop")
            break;
    }
    
    
}

int main(int argc, char const *argv[])
{
    Thread::ThreadFunc func(Thread_print);
    Thread thread(func);
    thread.start();
    // {
    //     MutexLockGuard lock(mutex);
    //     std::cout<<CurrentThread::tid()<<": "<<++num<<std::endl;
    // }
    // struct timespec abstime;
    // clock_gettime(CLOCK_REALTIME,&abstime);
    // time_t now=abstime.tv_sec;
    // Condition cond(mutex);
    // cond.waitForSeconds(3);
    // clock_gettime(CLOCK_REALTIME,&abstime);
    // std::cout<<abstime.tv_sec-now<<std::endl;
    std::string temp;
    while(std::cin>>temp){
        queue.put(temp);
        if(temp=="stop")
            break;
    }
    
    return 0;
}

