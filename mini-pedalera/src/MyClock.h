#ifndef MY_CLOCK_H
#define MY_CLOCK_H

#include <Arduino.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <ctime>

#include "Screen.h"

class MyClock
{
  public:
    MyClock();
    void init(Screen *screen);
    static void setDatetime(int datetime);
    void startClockMode();
    void clockMode(uint8_t wait_seconds);
  private:
    Screen *screen;

    const uint16_t RELEASE_TIME = 1000;
    const uint8_t _exit = 8;
    int minutes = -1;

    static void setSpainDatetime(time_t current_time);
    void showClock();
};

#endif