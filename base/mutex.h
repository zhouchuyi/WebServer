#ifndef BASE_MUTEX_H_
#define BASE_MUTEX_H_

#include<pthread.h>
#include<assert.h>
#include<sys/types.h>
#include"CurrentThread.h"
#include"noncopyable.h"
#include"Condition.h"
class MutexLockGuard;

class MutexLock:noncopyable
{
private:
    void assignHolder(){
        holder_=CurrentThread::tid();
    }
    void unassignHolder(){
        holder_=0;
    }
    void lock(){
        pthread_mutex_lock(&mutex_);
        assignHolder();
    }
    void unlock(){
        unassignHolder();
        pthread_mutex_unlock(&mutex_);
    }
    pthread_mutex_t * getThreadMutex(){
        return &mutex_;
    }
public:
    MutexLock():holder_(0){
        pthread_mutex_init(&mutex_,NULL);
    };
    ~MutexLock(){
        assert(holder_==0);
        pthread_mutex_destroy(&mutex_);
    };
    bool isLockedByThisThread() const {
        return holder_==CurrentThread::tid();
    }
    void assertLocked() const{
        assert(isLockedByThisThread());
    }
    

private:
    friend class MutexLockGuard;
    friend class Condition;
    pid_t holder_;
    pthread_mutex_t mutex_;
};

class MutexLockGuard:noncopyable
{
private:
    MutexLock & mutex_;
public:
    explicit MutexLockGuard(MutexLock &mutex)
    :mutex_(mutex)
    {mutex_.lock();};
    ~MutexLockGuard()
    {mutex_.unlock();};
};



#endif