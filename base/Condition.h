#ifndef BASE_CONDITION_H_
#define BASE_CONDITION_H_
#include"noncopyable.h"
#include"mutex.h"
#include<time.h>
#include<errno.h>
class Condition:noncopyable
{
private:
    
public:
    explicit Condition(MutexLock &mutex)
    : mutex_(mutex)
    {pthread_cond_init(&con,NULL);};
    ~Condition()
    {pthread_cond_destroy(&con);};
    
    void wait()
    {
        MutexLock::UnassignGuard ug(mutex_);
        pthread_cond_wait(&con,mutex_.getThreadMutex());
    }
    
    void notify()
    {
        pthread_cond_signal(&con);
    }
    
    void notifyAll()
    {
        pthread_cond_broadcast(&con);
    }

    bool waitForSeconds(int seconds)
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME,&abstime);
        abstime.tv_sec+=static_cast<time_t>(seconds);
        return ETIMEDOUT==pthread_cond_timedwait(&con,mutex_.getThreadMutex(),&abstime);
    }
private:
    MutexLock &mutex_;
    pthread_cond_t con;
};


#endif