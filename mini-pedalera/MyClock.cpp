#include "MyClock.h"

MyClock::MyClock()
{
}

void MyClock::init(Screen *screen)
{
  this->screen = screen;
}

void MyClock::startClockMode()
{
  screen->clean();
  screen->showClockBackground();
}

void MyClock::setDatetime(int datetime)
{
  time_t current_time = static_cast<time_t>(datetime);
  setSpainDatetime(current_time);
}

void MyClock::setSpainDatetime(time_t current_time)
{
  TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};
  TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};
  Timezone europeMadrid(CEST, CET);

  time_t local_time = europeMadrid.toLocal(current_time);
  setTime(local_time);
}

void MyClock::clockMode(uint8_t wait_seconds)
{
  bool clock_mode = true;
  unsigned long start_time = millis();
  showClock();
  while (clock_mode) {
    if (millis() - start_time >= wait_seconds * RELEASE_TIME) {
      clock_mode = false;
    }
  }
}

void MyClock::showClock()
{
  time_t current_time = now();
  screen->showClock(hour(current_time), minute(current_time), second(current_time), day(current_time), month(current_time), year(current_time));
}