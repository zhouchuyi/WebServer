#ifndef BASE_LOGSTREAM_H_
#define BASE_LOGSTREAM_H_

#include"mutex.h"
#include"noncopyable.h"
#include<string.h>
#include<string>
const int kSmallBuffer=4000;
const int kLargeBuffer=4000*1000;

template<int SIZE>
class FixedBuffer:noncopyable
{
public:
    FixedBuffer()
      : cur(data_)
    {

    }
    ~FixedBuffer()
    {

    }

    void append(const char* buf,size_t len)
    {
        if(static_cast<size_t>(avail())>len)
        {   
            memcpy(cur,buf,len);
            cur+=len;
        }
    }

    const char *data() const{return data_;}
    int avail() const{return static_cast<int>(end()-cur);}
    //write on cur directly
    char * current(){return cur;}
    int length() const {return static_cast<int>(cur-data_);}
    void add(size_t len){cur+=len;}
    
    void reset(){cur=data_;}
    void bzero(){memset(data_,0,sizeof(data_));}

    std::string toString()const{return std::string(data_,length());}    
private:
    
    char data_[SIZE];
    char *cur;
    const char *end()const{return data_+sizeof(data_);}
};



class LogStream:noncopyable
{
private:
    typedef FixedBuffer<kSmallBuffer> Buffer;
public:
    LogStream()=default;
    ~LogStream()=default;

    LogStream & operator<<(bool);


    LogStream & operator<<(short);
    LogStream & operator<<(unsigned short);

    LogStream & operator<<(int);
    LogStream & operator<<(unsigned int);

    LogStream & operator<<(long);
    LogStream & operator<<(unsigned long);
    LogStream & operator<<(long long);
    LogStream & operator<<(unsigned long long);

    LogStream & operator<<(const void *);

    LogStream & operator<<(double);

    LogStream & operator<<(float);

    LogStream & operator<<(const char *str)
    {
        if(str)
        {
            buffer_.append(str,strlen(str));
        }
        else
        {
            buffer_.append("(null)",6);
        }
        return *this;
        
    }

    LogStream & operator<<(const unsigned char *str)
    {
        *this<<reinterpret_cast<const char *>(str);
        return *this;
    }

    LogStream & operator<<(const std::string &str)
    {
        buffer_.append(str.c_str(),str.size());
        return *this;
    }

    LogStream & operator<<(const Buffer &buffer)
    {
        *this<<buffer.toString();
        return *this;
    }

    void append(const char*data,size_t len){buffer_.append(data,len);}

    const Buffer & buffer()const{return buffer_;}

    void resetBuffer(){buffer_.reset();}
private:

    template<typename T>
    void formatInterger(T);

    Buffer buffer_;

    static const int kMaxNumericSize; 
};

// template<typename T>
// size_t convert(char buf[],T value);




#endif