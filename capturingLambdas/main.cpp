#include "Arduino.h"
#include "PIT.h"

PIT timerArray[4];

void onTimer(int i)
{
    Serial.printf("PIT%d\n", i);
}


void setup()
{
    // setup 4 timer channels, each channel invokes the same callback and passes the channel number as parameter to the callback
    // Need to capture ch to achieve this
    for (int ch = 0; ch < 4; ch++)
    {
        timerArray[ch].begin([ch] { onTimer(ch); }, 1'000'000 * (ch+1), ch);
    }
}

void loop() {}
