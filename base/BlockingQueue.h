#ifndef BASE_BLOCKINGQUEUE_H_
#define BASE_BLOCKINGQUEUE_H_
#include<deque>
#include<assert.h>
#include"mutex.h"
#include"Condition.h"

template<typename T>
class BlockingQueue:noncopyable
{
private:
    std::deque<T> queue_;
    mutable MutexLock mutex_;
    Condition notEmpty_;
public:
    BlockingQueue()
    : mutex_(),
      notEmpty_(mutex_),
      queue_()
    {

    }

    ~BlockingQueue()=default;

    void put(const T &x)
    {
        MutexLockGuard lock(mutex_);
        queue_.push_back(x);
        notEmpty_.notify();
    }

    void put(T &&x)
    {
        MutexLockGuard lock(mutex_);
        queue_.push_back(std::move(x));
        notEmpty_.notify();
    }

    T take()
    {
        MutexLockGuard lock(mutex_);
        while(queue_.empty())
        {
            notEmpty_.wait();
        }
        assert(!queue_.empty());
        T front(std::move(queue_.front()));
        queue_.pop_front();

        return std::move(front);
        
    }

    size_t size() const{
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }
};




#endif