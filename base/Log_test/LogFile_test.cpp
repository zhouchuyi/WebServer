#include"../LogFile.h"
#include<string>
const char str[]="1234567890`1`12431245\n";
int main(int argc, char const *argv[])
{
    LogFile lf(std::string("logfile"));
    for(size_t i = 0; i < 10; i++)
    {
        /* code */
        lf.append(str,sizeof(str));
    }
    
    return 0;
}
