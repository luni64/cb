#pragma once

struct SimpleDelegate
{
    virtual void invoke() = 0;
};

template <typename T>
class SimpleDelegate_mfp : public SimpleDelegate
{
 public:
    using mfp_t = void (T::*)(); // type of a pointer to a void(void) member function of a class T

    SimpleDelegate_mfp(mfp_t mfp, T* obj)
    {
        this->mfp = mfp;
        this->obj = obj;
    }

    void invoke()
    {
        (obj->*mfp)();
    }

 protected:
    mfp_t mfp; // holds the memberfunction pointer passed in
    T* obj;    // holds a pointer to the instance on which the memberfunction is invoked
};


class SimpleDelegate_fp : public SimpleDelegate
{
 public:
    using fp_t = void (*)(); // type of a pointer to a void(void)  function

    SimpleDelegate_fp(fp_t fp)
    {
        this->fp = fp;
    }

    void invoke()
    {
        fp();
    }

 protected:
    fp_t fp; // holds the unction pointer passed in
};
