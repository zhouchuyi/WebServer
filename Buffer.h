#ifndef BUFFER_H_
#define BUFFER_H_

#include"base/noncopyable.h"
#include<string>
#include<vector>
#include<algorithm>

#include<string.h>
#include<assert.h>

class Buffer:noncopyable
{
public:
    static const int kPrepend;
    static const int kInitialSize;
    Buffer(size_t initialSize=kInitialSize)
     :buffer_(initialSize+kPrepend),
     readIndex_(kPrepend),
     writeIndex_(kPrepend)
     {
         assert(readableBytes()==0);
         assert(writableBytes()==initialSize);
         assert(prependableBytes()==kPrepend);
     }
    
    ~Buffer()=default;

    void swap(Buffer &rhs)
    {
        buffer_.swap(rhs.buffer_); 
        std::swap(readIndex_,rhs.readIndex_);
        std::swap(writeIndex_,rhs.writeIndex_);
    }

    size_t readableBytes()const
    {
        return writeIndex_-readIndex_;
    }

    size_t writableBytes()const
    {
        return buffer_.size()-writeIndex_;
    }

    size_t prependableBytes()const
    {
        return readIndex_;
    }
    const char* peek()const
    {
        return begin()+readIndex_;
    }
    const char* findCRLF()const
    {
        const char* crlf=std::search(peek(),beginWrite(),kCRLF,kCRLF+2);
        return crlf==beginWrite() ? nullptr : crlf;
    }

    const char* findCRLF(const char *start)const
    {
        assert(start >= peek() && start <= beginWrite());
        const char* crlf=std::search(start,beginWrite(),kCRLF,kCRLF+2);
        return crlf==beginWrite() ? nullptr : crlf;

    }
    const char* findEOL() const
    {
        const void* eol=memchr(peek(),'\n',readableBytes());
        return static_cast<const char*>(eol);
    }
    const char* findEOL(const char* start) const
    {
        assert(start >= peek() && start <= beginWrite());
        const void* eol=memchr(start,'\n',readableBytes());
        return static_cast<const char*>(eol);
    }
    void retrieve(size_t len)
    {
        assert(len<=readableBytes());
        if(len<readableBytes())
        {
            readIndex_+=len;
        }
        else
        {
            retriveAll();
        }
        
    }

    void retriveAll()
    {
        readIndex_=kPrepend;
        writeIndex_=kPrepend;
    }

    std::string retrieveAsString(size_t len)
    {
        assert(len<=readableBytes());
        std::string res(peek(),len);
        retrieve(len);
        return std::move(res);      
    }

    void append(const std::string &str)
    {
        ensureWritableBytes(str.size());
        append(str.data(),str.size());
    }
    void append(const char* data,size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data,data+len,beginWrite());
        hasWritten(len);
    }

    void append(const void* data,size_t len)
    {
        append(static_cast<const char*>(data),len);
    }

    void ensureWritableBytes(size_t len)
    {
        if(len>writableBytes())
        {
            makeSpace(len);
        }
    }
    char* beginWrite()
    {
        return begin()+writeIndex_;
    }

    const char* beginWrite()const
    {
        return begin()+writeIndex_;
    }

    void hasWritten(size_t len)
    {
        assert(len<=writableBytes());
        writeIndex_+=len;
    }

    void unwrite(size_t len)
    {
        assert(len<=readableBytes());
        writeIndex_-=len;
    }

    void prepend(const void* data,size_t len)
    {
        assert(len<=prependableBytes());
        readIndex_-=len;
        const char* s=static_cast<const char *>(data);
        std::copy(s,s+len,begin()+readIndex_);
        
    }

    ssize_t readFd(int fd,int *savedErrno);

private:
    char *begin()
    {
        return &*buffer_.begin();
    }

    const char* begin() const
    {
        return &*buffer_.begin();
    }

    void makeSpace(size_t len)
    {
        if(len>writableBytes()+prependableBytes())
        {
            buffer_.resize(writeIndex_+len);
        }
        else
        {
            assert(readIndex_>kPrepend);
            size_t readable=readableBytes();
            std::copy(begin()+readIndex_,begin()+writeIndex_,begin()+kPrepend);
            readIndex_=kPrepend;
            writeIndex_=readIndex_+readable;
        }
        
    }


private:    
    std::vector<char> buffer_;
    size_t readIndex_;
    size_t writeIndex_;
    static const char kCRLF[];
};




#endif