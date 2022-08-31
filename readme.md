** This is just prove of concept code. There is no error checking code implemented and there are most probably are some bugs in it**

This is just a collection of examples to show a possible implementation of a new callback system
See https://forum.pjrc.com/threads/70986-Lightweight-C-callbacks?p=311798&viewfull=1#post311798

Key requirements
1) avoiding heap allocation
2) ability to call C functions
3) static C++ functions
4) class member functions
5) Avoiding C++ template syntax in Arduino sketch code is highly desirable
6) template syntax is ok inside libraries and their headers, and maybe we can adopt a practice of typedefs or #defines for the function def).
7) Support for lambda functions would be nice, maybe even mutable lambda functions.

To have a real example to play with, a very simple implementation of a PIT timer class (Teensy 4.x) using the proposed callback system is implemented.


Example1 demonstrates how 2) 3) 4) 5) 6) and 7) (only capture less lambdas) is fulfilled.
Example2 demonstrates that the callback system can be used to embed callback providers (here PIT) into user classes, which IMHO is the most important use case.

Generally, it is not as elegant as using a `std::function` based API but it is much cheaper and avoids dynamic memory allocation.

```c++
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
```