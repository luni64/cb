#include "Arduino.h"
#include "PIT.h"

// demonstrate how to embedd a PIT into a class
class Blinker
{
 public:
    void begin()
    {
        t1.begin(0, &Blinker::onTimer, this, 500'000); // attach the non static member function "onTimer" and invoke using "this"
    }

 protected:
    void onTimer()
    {
        digitalToggle(LED_BUILTIN);
    }

    PIT t1;
};

Blinker blinker; // some instance of Test


void setup()
{
    while (!Serial) {}
    pinMode(LED_BUILTIN, OUTPUT);

    blinker.begin();
}

void loop() {}
