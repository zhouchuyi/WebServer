#include<iostream>
#include"../FileUtil.h"
char buffer[4096*4];
int main(int argc, char const *argv[])
{
    for(size_t i = 0; i < 4096*4; i++)
    {
        /* code */
        buffer[i]='a';
    }
    
    AppendFile file("file_test");
    
    file.append(buffer,4096*4);
    std::cout<<file.writtenBytes();
    return 0;
}
