#ifndef BASE_LOGFILE_H_
#define BASE_LOG_FILE_H_
#include"noncopyable.h"
#include"FileUtil.h"
#include"mutex.h"
#include<memory>

class LogFile: noncopyable
{
private:

public:
    typedef std::unique_ptr<AppendFile> AppendFilePtr;
    typedef std::unique_ptr<MutexLock> MutexLockPtr;
    
    LogFile(const std::string &basename,
    int flushInterval=64
    );
    ~LogFile();

    void append(const char *logline,size_t len);

    void flush(); 

private:

    void append_unlocked(const char *logline,size_t len);
    const std::string basename_;
    const int flushInterVal_;

    AppendFilePtr file_;
    MutexLockPtr mutex_;
    
    time_t lastFilsh_;
};



#endif