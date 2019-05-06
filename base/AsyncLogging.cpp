#include"AsyncLogging.h"
#include"LogFile.h"
AsyncLogging::AsyncLogging(const std::string &name,
int flushInterval)
 : name_(name),
   running_(false),
   flushInterval_(flushInterval),
   thread_(std::bind(&AsyncLogging::threadFunc,this),"Logging"),
   latch_(1),
   mutex_(),
   cond_(mutex_),
   currentBuffer_(new Buffer),
   nextBuffer_(new Buffer),
   buffers()
   {
       currentBuffer_->bzero();
       nextBuffer_->bzero();
       buffers.reserve(16);
   }
void AsyncLogging::start()
{
    assert(!running_);
    running_=true;
    thread_.start();
    latch_.wait();
}

void AsyncLogging::stop()
{
    assert(running_);
    running_=false;
    cond_.notifyAll();
    thread_.join();
}
AsyncLogging::~AsyncLogging()
{
    if(running_)
    {
        stop();
    }
}

void AsyncLogging::append(const char* logline,size_t len)
{
    MutexLockGuard lock(mutex_);
    
    if(currentBuffer_->avail()>len)
    {
        currentBuffer_->append(logline,len);
    }
    else
    {
        buffers.emplace_back(currentBuffer_.release());
        assert(!currentBuffer_);
        if(nextBuffer_)
        {
            currentBuffer_.swap(nextBuffer_);
            assert(!nextBuffer_);
            assert(currentBuffer_);
            currentBuffer_->append(logline,len);
        }
        else
        {
            currentBuffer_.reset(new Buffer);
            currentBuffer_->append(logline,len);
        }
        
    }
    cond_.notify();
    
}

void AsyncLogging::threadFunc()
{
    assert(running_=true);
    latch_.CountDown();
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    LogFile output(name_);
    BufferVector buffersToWrite;
    newBuffer1->bzero();
    newBuffer2->bzero();
    buffersToWrite.reserve(16);
    while(running_){
        assert(buffersToWrite.empty());
        assert(newBuffer1 && newBuffer1->length()==0);
        assert(newBuffer2 && newBuffer2->length()==0);
        {
            MutexLockGuard lock(mutex_);
            if(buffers.empty())
            {
                cond_.waitForSeconds(flushInterval_);
            }
            buffers.emplace_back(currentBuffer_.release());
            currentBuffer_.swap(newBuffer1);
            assert(!newBuffer1&&currentBuffer_);
            buffersToWrite.swap(buffers);
            assert(buffers.empty());
            if(!nextBuffer_)
            {
                nextBuffer_.swap(newBuffer2);
                assert(!newBuffer2&&nextBuffer_);
        
            }
        }
        assert(!buffersToWrite.empty());
        for(const auto& buffer : buffersToWrite)
        {
            output.append(buffer->data(),buffer->length());
        }
        
        if(buffersToWrite.size()>2)
        {
            buffersToWrite.resize(2);
        }

        if(!newBuffer1)
        {
            assert(!buffersToWrite.empty());
            newBuffer1.swap(buffersToWrite.back());
            buffersToWrite.pop_back();
            assert(newBuffer1);
            newBuffer1->reset();
        }
        if(!newBuffer2)
        {
            assert(!buffersToWrite.empty());
            newBuffer2.swap(buffersToWrite.back());
            buffersToWrite.pop_back();
            assert(newBuffer2);
            newBuffer2->reset();
            
        }
        buffersToWrite.clear();
        output.flush();
    }
    output.flush();
}