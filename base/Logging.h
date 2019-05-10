#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include"AsyncLogging.h"
#include"LogFile.h"
#include"LogStream.h"

class Logger
{
public:

    Logger(const std::string &basename,int line);
    ~Logger();
    LogStream & stream()
    {
        return Iml_.stream_;
    }
    static std::string getFilename()
    {
        return Logfilename_;
    }; 

    static void setLogFilename(std::string &Logfilename)
    {
        Logfilename_=Logfilename;
    }
private:

    class Iml
    {
    public:        
        LogStream stream_;//log stream
        std::string basename_;//logfile basename
        void formatTime();//format time
        Iml(const std::string &name,int line);
        ~Iml();
        int line_;
    };

    Iml Iml_;

    static std::string Logfilename_;//save log to file


};


#define Log Logger(__FILE__,__LINE__).stream() 

#endif