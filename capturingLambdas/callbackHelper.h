#pragma once
#include "Arduino.h"
#include "simpleDelegate.h"
#include <memory>

/**
 * To avoid dynamic memory allocation we need to prealocate static memory and construct the objects into this using 'placement new'
 * Memory usage:
 *   function pointer delegate: 4bytes
 *   member function pointer delegate: 16bytes
 *   capture less lambda: 1 byte
 *   capturing lambda: total size of captured parameters.
 **/

 constexpr size_t slotSize = 16; // should be >= 16 to have enough space for member function pointer delegates
                                 // increase to allow for more captured parameters. Code will generate compile error if user captures more than this value

template <unsigned nrOfSlots>
class CallbackHelper
{
 public:
    template <typename T>
    SimpleDelegate* makeCallback(void (T::*mfp)(), T* obj, unsigned slot)
    {
        return new (arena + slot * slotSize) SimpleDelegate_mfp<T>(mfp, obj); // generate a delegate on the static memory arena and return a pointer to it
    }

    SimpleDelegate* makeCallback(void (*fp)(), unsigned slot)
    {
        return new (arena + slot * slotSize) SimpleDelegate_fp(fp); // placement new does not allocate memory!
    }

    template <typename T>
    SimpleDelegate* makeCallback(T& l, unsigned slot)
    {
        static_assert(sizeof(T) <= slotSize, "Not enough prealocated memory, please adjust");
        return new (arena + slot * slotSize) SimpleDelegate_lambda<T>(l); // placement new does not allocate memory!
    }

 protected:
    static uint8_t arena[slotSize * nrOfSlots]; // since we want to avoid dynamic memory allocation we need to statically reserve the required memory for the delegates
};

// initialize static variables
template <unsigned nrOfSlots>
uint8_t CallbackHelper<nrOfSlots>::arena[slotSize * nrOfSlots]; // sizeof(SimpleDelegate<T>) = 16;
