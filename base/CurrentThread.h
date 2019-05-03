#ifndef BASE_CURRENTTHREAD_H_
#define BASE_CURRENTTHREAD_H_

namespace CurrentThread
{
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread const char * t_threadName;
    extern __thread int t_tidStringLength;
    extern void cacheTid();
    inline int tid(){
        if(t_cachedTid==0){
            cacheTid();
        }
        return t_cachedTid;
    }
    inline char * tidString(){
        return t_tidString;
    }
    inline int tidStringLength(){
        return t_tidStringLength;
    }
    inline const char * thredName(){
        return t_threadName;
    }
} // CurrnetThread


#endif