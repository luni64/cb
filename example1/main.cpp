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

    //!!! The begin function defaults to PIT timer channel 0. You can use e.g channel 3 by timer.begin(onTimer, 250'000, 3); !!!

    use free function callback
    //timer.begin(onTimer, 250'000);

    // use lambda to attach member function as callback
    // timer.begin([] { test.myFunc2(); }, 250'000);

    // use member function pointer and instance to attach callback
    // timer.begin(&Test::myFunc1, &test, 250'000, 0);

    // attach non capturing lambda function as callback
    // timer.begin([] { Serial.println("called lambda"); }, 250'000);

    // attach capturing lambda function as callback
    // int n = 42;
    // timer.begin([n] { Serial.println(n); }, 500'000 );
}

void loop() {}
