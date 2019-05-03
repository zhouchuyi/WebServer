#ifndef BASE_THREAD_H_
#define BASE_THREAD_H_
#include"CountDownLatch.h"
#include<functional>
#include<memory>
#include<string>

pid_t gettid();
class Thread
{
public:
    typedef std::function<void()> ThreadFunc;

    void start();
    int join();
    bool started()const{
        return started_;
    }
    bool joined()const{
        return joined_;
    }
    pid_t tid()const{
        return tid_;
    }
    const std::string& name()const{
        return name_;
    }
private:
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    std::string name_;
    ThreadFunc func_;
    CountDownLatch latch_;
    void setDefaultName();
public:
    explicit Thread(ThreadFunc,const std::string &name=std::string());
    ~Thread();
};




#endif