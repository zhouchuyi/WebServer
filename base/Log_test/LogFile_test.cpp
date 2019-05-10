// #include"../LogFile.h"
// #include"../LogStream.h"
// #include"../AsyncLogging.h"
#include"../Logging.h"
#include<string>
#include<iostream>
#include<unistd.h>
const char str[]="2019-5-26-zcy";
int main(int argc, char const *argv[])
{
    for(size_t i = 0; i < 10; i++)
    {
        /* code */
        Log<<str<<"\n";

    }
    sleep(4);
    
    
    
    return 0;
}
