#include"../LogFile.h"
#include"../LogStream.h"
#include<string>
#include<iostream>
const char str[]="1234567890`1`12431245\n";
int main(int argc, char const *argv[])
{
    // FixedBuffer<kSmallBuffer> buffer;
    // buffer.append("123456",6);
    // std::cout<<buffer.toString()<<" "<<buffer.avail()<<" "<<buffer.length()<<std::endl;
    // buffer.reset();
    // buffer.append("aaa",3);
    // std::cout<<buffer.toString()<<" "<<buffer.avail()<<" "<<buffer.length()<<std::endl;
    LogStream log;
    log<<int(123)<<"\n";
    log<<double(-1123.56)<<"\n";
    log<<"please\n";
    const FixedBuffer<kSmallBuffer> &buffer=log.buffer();
    std::cout<<buffer.toString()<<std::endl;
    return 0;
}
