#include"CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
: mutex_(),count_(count),cond_(mutex_)
{}

CountDownLatch::~CountDownLatch()=default;

void CountDownLatch::wait(){
    MutexLockGuard lock(mutex_);
    while(count_>0){
        cond_.wait();
    }
    
}

int CountDownLatch::getCount(){
    MutexLockGuard lock(mutex_);
    return count_;
}

void CountDownLatch::CountDown(){
    MutexLockGuard lock(mutex_);
    --count_;
    if(count_==0)
        cond_.notifyAll();

}