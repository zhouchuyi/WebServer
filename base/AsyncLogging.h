#ifndef BASE_ASYNCLOGGING_H_
#define BASE_ASYNCLOGGING_H_

#include"Thread.h"
#include"noncopyable.h"
#include"mutex.h"
#include"LogStream.h"
#include<vector>
#include<memory>
#include<functional>
#include<atomic>
class AsyncLogging:noncopyable
{
private:

public:
    AsyncLogging(const std::string &name,
    int flushInterval=3
    );
    ~AsyncLogging();
    void append(const char *logline,size_t len);
    void start();
    void stop();

private:
    
    void threadFunc();

    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::unique_ptr<Buffer> BufferPtr;
    typedef std::vector<BufferPtr> BufferVector;

    const int flushInterval_;
    std::atomic<bool> running_;
    Thread thread_;
    const std::string name_;
    CountDownLatch latch_;    
    MutexLock mutex_;
    Condition cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers;
};





















#endif