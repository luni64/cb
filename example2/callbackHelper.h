#pragma once
#include "Arduino.h"
#include "simpleDelegate.h"
#include <memory>

template <unsigned nrOfSlots>
class CallbackHelper
{
 public:
    template <typename T>
    SimpleDelegate* makeCallback(void (T::*mfp)(), T* obj, unsigned slot)
    {
        return new (arena + slot * 16) SimpleDelegate_mfp<T>(mfp, obj); // generate a delegate on the static memory arena and return a pointer to it
    }

    SimpleDelegate* makeCallback(void (*fp)(), unsigned slot)
    {
        return new (arena + slot * 16) SimpleDelegate_fp(fp); // placement new does not allocate memory!
        return nullptr;
    }

 protected:
    static uint8_t arena[16 * nrOfSlots]; // since we want to avoid dynamic memory allocation we need to statically reserve the required memory for the delegates
};

template <unsigned nrOfSlots>
uint8_t CallbackHelper<nrOfSlots>::arena[16 * nrOfSlots]; // sizeof(SimpleDelegate<T>) = 16;
