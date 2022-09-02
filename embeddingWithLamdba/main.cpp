#include "Arduino.h"
#include "PIT.h"

// demonstrate how to embed a PIT into a class
class Blinker
{
 public:
    void begin(unsigned pin, const char* msg)
    {
        t1.begin([this, pin] { onTimer(pin); }, 150'000, 0);  //  attach a non static member function with parameter, need to capture 'this' and the parameter
        t2.begin([msg] { Serial.println(msg); }, 500'000, 1); // attach free standing lambda with string parameter captured
    }

 protected:
    void onTimer(unsigned blinkPin)
    {
        digitalToggle(blinkPin);
    }

    PIT t1, t2;
    unsigned bp, mp;
};

Blinker blinker;


void setup()
{
    while (!Serial) {}
    pinMode(LED_BUILTIN, OUTPUT);

    blinker.begin(LED_BUILTIN, "Hello PIT");
}

void loop() {}
