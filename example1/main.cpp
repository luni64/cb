#include "Arduino.h"
#include "PIT.h"

// test class to demonstrate member function callbacks
class Test
{
 public:
    void myFunc1()
    {
        Serial.printf("called Test::myFunc1 i = %d\n", i);
    }
    void myFunc2()
    {
        Serial.printf("called Test::myFunc2 2*i = %d\n", i * 2);
    }

    int i = 0;
};
Test test{42}; // some instance of Test

// free function callback
void onTimer()
{
    Serial.println("called free function");
}

PIT timer;

void setup()
{
    while (!Serial) {}

    constexpr unsigned PIT_channel = 0; // for the sake of simplicity the PIT class doesn't automatically choose a free channel. (0..3)

    // use free function callback
    timer.begin(0, onTimer, 250'000);

    // attach lambda function as callback
    //timer.begin(PIT_channel, [] { Serial.println("called lambda"); }, 250'000);

    // use lambda to attach member function as callback
    // timer.begin(PIT_channel, [] { test.myFunc2(); }, 250'000);

    // use member function pointer and instance to attach callback
    //timer.begin(PIT_channel, &Test::myFunc1, &test, 250'000);
}

void loop() {}
