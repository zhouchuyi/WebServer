#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<endian.h>
#include"../Buffer.h"
int main(int argc, char const *argv[])
{
    Buffer buffer_;
    std::string str("asdfghjkl");
    buffer_.append(str.data(),str.size());
    int32_t len=static_cast<int32_t>(buffer_.readableBytes());
    int32_t be32=::htobe32(len);
    buffer_.prepend(&be32,sizeof be32);
    const void *s=buffer_.peek();
    int32_t t_be32=*static_cast<const int32_t*>(s);
    std::cout<<be32toh(t_be32);
    // std::cout<<sizeof a<<" "<<buffer_.prependableBytes()<<" "<<buffer_.readableBytes();
    // buffer_.append(str,strlen(str));
    // std::cout<<buffer_.readableBytes()<<buffer_.writableBytes()<<std::endl;
    // std::cout<<strlen(buffer_.findCRLF())<<std::endl;
    return 0;
}
