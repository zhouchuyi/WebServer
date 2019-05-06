#include"../LogFile.h"
#include"../LogStream.h"
#include"../AsyncLogging.h"
#include<string>
#include<iostream>
#include<unistd.h>
const char str[]="2019-5-26-zcy\n";
int main(int argc, char const *argv[])
{
    AsyncLogging log(std::string("new"));
    log.start();
    for(size_t i = 0; i < 60; i++)
    {
        sleep(1);
        /* code */
        log.append(str,strlen(str));
    }
    
    
    return 0;
}
