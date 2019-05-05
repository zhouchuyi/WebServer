#include"FileUtil.h"

AppendFile::AppendFile(std::string Filename)
:fp(::fopen(Filename.c_str(),"ae")),
writtenBytes_(0)
{
    assert(fp);
    setbuffer(fp,buffer_,sizeof(buffer_));
}

AppendFile::~AppendFile()
{
    fclose(fp);
}

void AppendFile::flush(){
    fflush(fp);
}

size_t AppendFile::write(const char *logline,size_t len)
{
    return fwrite_unlocked(logline,sizeof(char),len,fp);
}

void AppendFile::append(const char *logline,size_t len)
{
    size_t n=write(logline,len);
    size_t remain=len-n;
    while(remain>0)
    {
        size_t x=write(logline+n,remain);
        if(x==0)
        {
            int err=ferror(fp);
            if(err)
            {
                fprintf(stderr,"AppendFile::appen() failed");
            }

            break;
        }
        n+=x;
        remain=len-n;
    }
    
    writtenBytes_+=len;
}
