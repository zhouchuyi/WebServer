#ifndef BASE_NONCOPYABLE_H_
#define BASE_NONCOPYABLE_H_


class noncopyable
{
public:
    /* data */
    noncopyable(const noncopyable &)=delete;
    noncopyable & operator=(const noncopyable &)=delete;
protected:
    noncopyable()=default;
    ~noncopyable()=default;
};




#endif