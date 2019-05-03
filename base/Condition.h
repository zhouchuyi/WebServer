#ifndef BASE_CONDITION_H_
#define BASE_CONDITION_H_
#include"noncopyable.h"
#include"mutex.h"
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
        pthread_cond_wait(&con,mutex_.getThreadMutex());
    }
    void notify()
    {
        pthread_cond_signal(&con);
    }
    void notifyAll(){
        pthread_cond_broadcast(&con);
    }
private:
    MutexLock &mutex_;
    pthread_cond_t con;
};


#endif