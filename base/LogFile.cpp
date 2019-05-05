#include"LogFile.h"

LogFile::LogFile(const std::string &basename,
int flushInterval)
: basename_(basename),
flushInterVal_(flushInterval),
file_(new AppendFile(basename)),
mutex_(new MutexLock()),
lastFilsh_(0)
{
    assert(basename.find('/')==std::string::npos);
}

LogFile::~LogFile()=default;

void LogFile::flush()
{
    MutexLockGuard lock(*mutex_);
    file_->flush();
}

void LogFile::append(const char *logline,size_t len)
{
    MutexLockGuard lock(*mutex_);
    append_unlocked(logline,len);
}

void LogFile::append_unlocked(const char *logline,size_t len)
{
    file_->append(logline,len);
    time_t now=::time(NULL);
    if(now-lastFilsh_>flushInterVal_)
    {
        lastFilsh_=now;
        file_->flush();
    }
}

