#pragma once
#include "callbackHelper.h"

class PIT : protected CallbackHelper<4> // derive from CallbackHelper, pass required number of callback slots as template paramter
{
 public:
    void begin(void (*callback)(), float period, unsigned ch = 0) // normal free function, static member function, captureless lamdas
    {
        callbacks[ch] = makeCallback(callback, ch); // generate a delegate from the paramters and store it for calling from the ISR
        initPIT();                                  // setup PIT module
        startChannel(ch, period);
    }

    template <class T>
    void begin(void (T::*mfp)(), T* obj, float period, unsigned ch = 0) // member function pointer version
    {
        callbacks[ch] = makeCallback(mfp, obj, ch); // make a delegate from the parameters and store for calling from the ISR
        initPIT();                                  // setup PIT module
        startChannel(ch, period);
    }

    template <class T>
    void begin(T lambda, float period, unsigned ch = 0) // member function pointer version
    {
        callbacks[ch] = makeCallback(lambda, ch); // make a delegate from the parameters and store for calling from the ISR
        initPIT();                                // setup PIT module
        startChannel(ch, period);
    }

 protected:
    static void isr() // T4 PIT shares interrupt for all 4 channels
    {
        for (int ch = 0; ch < 4; ch++)
        {
            if (IMXRT_PIT_CHANNELS[ch].TFLG) // was the channel triggerd?
            {
                IMXRT_PIT_CHANNELS[ch].TFLG = 1; // reset the interrupt flag
                callbacks[ch]->invoke();         // invoke the stored callback
            }
            asm volatile("dsb"); // wait until register changes propagated through the cache to avoid double calls
        }
    }

    void initPIT()
    {
        static bool isInitialized = false;

        if (!isInitialized)
        {
            CCM_CCGR1 |= CCM_CCGR1_PIT(CCM_CCGR_ON);
            PIT_MCR = 1;

            attachInterruptVector(IRQ_PIT, isr);
            NVIC_ENABLE_IRQ(IRQ_PIT);

            isInitialized = true;
        }
    }

    void startChannel(unsigned ch, float period)
    {
        IMXRT_PIT_CHANNELS[ch].LDVAL = period * 24;                   // reload value, period in microseconds, timer clock 24MHz
        IMXRT_PIT_CHANNELS[ch].TCTRL = PIT_TCTRL_TEN | PIT_TCTRL_TIE; // enable channel and generate interrupts on overflow
    }

    static SimpleDelegate* callbacks[4]; // holds the callback delegates
};

SimpleDelegate* PIT::callbacks[4];