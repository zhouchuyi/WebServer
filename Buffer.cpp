#include"Buffer.h"
#include<sys/uio.h>
#include<errno.h>
const char Buffer::kCRLF[]="\r\n";
const int Buffer::kPrepend=8;
const int Buffer::kInitialSize=1024;

ssize_t Buffer::readFd(int fd,int *savedErrno)
{
    char extrabuf[65536];
    struct iovec vec[2];
    int writable=writableBytes();
    vec[0].iov_base=beginWrite();
    vec[0].iov_len=writable;
    vec[1].iov_base=extrabuf;
    vec[1].iov_len=sizeof extrabuf;
    const int iovcnt=(writable<sizeof extrabuf)? 2 : 1;
    ssize_t n=::readv(fd,vec,iovcnt);
    if(n<0)
    {
        *savedErrno=errno;
    }
    else if (static_cast<size_t>(n)<=writable)
    {
        hasWritten(n);
    }
    else
    {
        hasWritten(writable);
        append(extrabuf,n-writable);
    }
    return n;
}