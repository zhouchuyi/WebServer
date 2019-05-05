#ifndef BASE_FILEUTIL_H_
#define BASE_FILEUTIL_H_
#include"noncopyable.h"
#include<assert.h>
#include<string>
#include<unistd.h>
#include<fcntl.h>
class AppendFile:noncopyable
{
private:
    /* data */
public:
    explicit AppendFile(std::string Filename);
    
    ~AppendFile();

    void append(const char *logline,size_t len);

    void flush();

    off_t writtenBytes() const{return writtenBytes_;}

private:

    size_t write(const char *logline,size_t len);

    FILE *fp;

    off_t writtenBytes_;

    char buffer_[64*1024];

};



#endif