#ifndef BASE_COUNTDOWNLATCH_H_
#define BASE_COUNTDOWNLATCH_H_

#include"mutex.h"
#include"Condition.h"

class CountDownLatch
{
private:
    Condition cond_;
    mutable MutexLock mutex_;
    int count_;

public:
    explicit CountDownLatch(int count);
    
    ~CountDownLatch();

    void wait();

    int getCount();

    void CountDown();
};



#endif