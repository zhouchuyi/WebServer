#include"LogStream.h"
#include<algorithm>
const int LogStream::kMaxNumericSize=32;
const char digits[]="9876543210123456789"; 
const char *zero=digits+9;
static_assert(sizeof(digits)==20,"wrong digits");

template<typename T>
size_t convert(char buf[],T value)
{
    T i=value;
    char *p=buf;
    do
    {
        *(p++)=*(static_cast<int>(i%10)+zero);
        i/=10;
    } while (i!=0);
    
    if(value<0)
    {
        *p++='-';
    }
    *p='\0';
    std::reverse(buf,p);
    return p-buf;
}
// template size_t convert(char*,int);

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;

template<typename T>
void LogStream::formatInterger(T value)
{
    if(buffer_.avail()>=kMaxNumericSize)
    {
        size_t len=convert(buffer_.current(),value);
        buffer_.add(len);
    }
}

LogStream & LogStream::operator<<(bool n)
{
    buffer_.append(n?"1":"0",1);
    return *this;
}


LogStream & LogStream::operator<<(short n)
{
    *this<<static_cast<int>(n);
    return *this;
}

LogStream & LogStream::operator<<(unsigned short n)
{
    *this<<static_cast<unsigned int>(n);
    return *this;
}

LogStream & LogStream::operator<<(int n)
{
    formatInterger(n);
    return *this;
}

LogStream & LogStream::operator<<(unsigned int n)
{
    formatInterger(n);
    return *this;
}

LogStream & LogStream::operator<<(long n)
{
    formatInterger(n);
    return *this;
}

LogStream & LogStream::operator<<(unsigned long n)
{
    formatInterger(n);
    return *this;
}

LogStream & LogStream::operator<<(long long n)
{
    formatInterger(n);
    return *this;
}

LogStream & LogStream::operator<<(unsigned long long n)
{
    formatInterger(n);
    return *this;
}

LogStream & LogStream::operator<<(double n)
{
    if(buffer_.avail()>kMaxNumericSize)
    {
        int len=snprintf(buffer_.current(),kMaxNumericSize,"%.12g",n);
        buffer_.add(len);
    }
    return *this;
}

LogStream & LogStream::operator<<(float n)
{
    *this<<static_cast<double>(n);
    return *this;
}
