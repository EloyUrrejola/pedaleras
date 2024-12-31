#include "MyClock.h"

MyClock::MyClock()
{
}

void MyClock::init(Screen *screen)
{
  this->screen = screen;
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