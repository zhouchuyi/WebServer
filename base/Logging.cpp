#include"Logging.h"
#include"time.h"
#include"Thread.h"
#include<sys/time.h>
std::string Logger::Logfilename_="WebServer_Log_File";

static AsyncLogging * asyncLogging_;
static pthread_once_t once=PTHREAD_ONCE_INIT;

void init()
{
    asyncLogging_=new AsyncLogging(Logger::getFilename());
    asyncLogging_->start();
}

void output(const char *logline,size_t len)
{
    pthread_once(&once,init);
    asyncLogging_->append(logline,len);
}

Logger::Iml::Iml(const std::string &basename,int line)
 : line_(line),
   basename_(basename),
   stream_()
   {
       formatTime();
   }

Logger::Iml::~Iml()=default;

Logger::Logger(const std::string &basename,int line)
 : Iml_(basename,line)
 {
     
 }


Logger::~Logger()
{
    Iml_.stream_<<Iml_.basename_<<" : "<<Iml_.line_<<" \n";
    const auto & buf=Iml_.stream_.buffer();
    output(buf.data(),buf.length());
}


void Logger::Iml::formatTime()
{
    struct timeval tv;
    time_t time;
    char str_t[26];
    gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);   
    strftime(str_t, sizeof str_t, "%Y-%m-%d %H:%M:%S\n", p_time);
    stream_ << str_t;
}