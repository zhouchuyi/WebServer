#include"../base/ThreadLocalSingleton.h"
#include"../base/Thread.h"
#include<stdio.h>
#include<functional>
class Test
{
public:
    Test()
    {
        printf("tid=%d, constructing %p\n",CurrentThread::tid(),this);
    }
    ~Test()
    {
        printf("tid=%d, destructing %p\n",CurrentThread::tid(),this);
    }

    const std::string& name()
    { return name_; }
    void setName(const std::string& name)
    { name_=name; }
private:
    std::string name_;


};


void ThreadFunc(const char* str)
{
    printf("tid=%d, %p name=%s\n",
    CurrentThread::tid(),
    &ThreadLocalSingleton<Test>::instace(),
    ThreadLocalSingleton<Test>::instace().name().c_str());
    ThreadLocalSingleton<Test>::instace().setName(str);
    printf("tid=%d, %p name=%s\n",
    CurrentThread::tid(),
    &ThreadLocalSingleton<Test>::instace(),
    ThreadLocalSingleton<Test>::instace().name().c_str());
}
int main(int argc, char const *argv[])
{
    ThreadLocalSingleton<Test>::instace().setName("main thread");
    Thread t1(std::bind(ThreadFunc,"thread 1"));
    Thread t2(std::bind(ThreadFunc,"thread 2"));
    t1.start();
    t2.start();
    t1.join();
    printf("tid=%d, %p name=%s\n",
    CurrentThread::tid(),
    &ThreadLocalSingleton<Test>::instace(),
    ThreadLocalSingleton<Test>::instace().name().c_str());
    t2.join();

    return 0;
}
