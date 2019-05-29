#ifndef BASE_THREADLOCALSINGLETON_H_
#define BASE_THREADLOCALSINGLETON_H_

#include"noncopyable.h"
#include<assert.h>
#include<pthread.h>

template<typename T>
class ThreadLocalSingleton:noncopyable
{
public:
    ThreadLocalSingleton()=delete;
    ~ThreadLocalSingleton()=delete;

    static T& instace()
    {
        if(p_value==NULL)
        {
            p_value=new T();
            deleter_.set(p_value);
        }
        return *p_value;
    }

    static T* pointer()
    {
        return p_value;
    }
private:

    class Deleter
    {
    public:
        Deleter()
        {
            pthread_key_create(&key,&ThreadLocalSingleton::destructor);
        }
        ~Deleter()
        {
            pthread_key_delete(key);
        }
        void set(T* obj)
        {
            assert(pthread_getspecific(key)==NULL);
            pthread_setspecific(key,obj);
        }
    private:
        pthread_key_t key;
    };
    
    
    static void destructor(void* obj)
    {
        assert(obj==p_value);
        delete p_value;
        p_value=NULL;        
    }

    static __thread T* p_value;
    static Deleter deleter_;


};


template<typename T>
__thread T* ThreadLocalSingleton<T>::p_value=NULL;


template<typename T>
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_; 








#endif