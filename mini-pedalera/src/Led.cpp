#include "Led.h"
#include "Status.h"

uint8_t Led::leds_mode = BASS_MODE;
uint8_t Led::led_intensity_level = 5;

Led::Led(uint8_t pin, uint8_t ccs[2])
{
  pinMode(pin, OUTPUT);
  led_pin = pin;
  led_ccs[0] = ccs[0];
  led_ccs[1] = ccs[1];
}

void Led::updateLedsMode(uint8_t new_mode)
{
  leds_mode = new_mode;
}

void Led::on()
{
  analogWrite(led_pin, led_intensity_level);
}

void Led::off()
{
  analogWrite(led_pin, LOW);
}

void Led::flash(int time_on, int time_off, int times)
{
  led_time_on = time_on;
  led_time_off = time_off;
  led_times = times;
  led_state = ON;
  led_time_start = millis();
  on();
}

void Led::flashUpdate()
{
  led_time_now = millis();
  if (led_state == ON) {
    if (led_time_now - led_time_start >= led_time_on) {
      off();
      led_time_start = millis();
      led_state = OFF;
      led_times--;
    }
  } else {
    if (led_time_now - led_time_start >= led_time_off) {
      on();
      led_time_start = millis();
      led_state = ON;
    }
  }
}

bool Led::flashUpdateTimes()
{
  led_time_now = millis();
  if (led_times != 0) {
    if (led_state == ON) {
      if (led_time_now - led_time_start >= led_time_on) {
        off();
        led_time_start = millis();
        led_state = OFF;
        led_times--;
      }
    } else {
      if (led_time_now - led_time_start >= led_time_off) {
        on();
        led_time_start = millis();
        led_state = ON;
      }
    }
  }
  if (led_times != 0) {
    return true;
  }
  return false;
}

uint8_t Led::getLedIntensityLevel()
{
  return Led::led_intensity_level;
}

void Led::setLedIntensityLevel(uint8_t intensity_level)
{
  Led::led_intensity_level = intensity_level;
}

uint8_t Led::getLedCc()
{
  return led_ccs[leds_mode];
}

void Led::setStatusLeds(Led **leds)
{
  const bool* params = Status::getAll();
  if (params[7]) {
    leds[0]->on();
  } else {
    leds[0]->off();
  }
  if (params[0]) {
    if (params[8]) {
      leds[1]->on();
    } else {
      leds[1]->off();
    }
    if (params[9]) {
      leds[2]->on();
    } else {
      leds[2]->off();
    }
    if (params[10]) {
      leds[3]->on();
    } else {
      leds[3]->off();
    }
  } else {
    if (params[2]) {
      leds[1]->on();
    } else {
      leds[1]->off();
    }
    if (params[3]) {
      leds[2]->on();
    } else {
      leds[2]->off();
    }
    if (params[4]) {
      leds[3]->on();
    } else {
      leds[3]->off();
    }
  }
  if (params[6]) {
    leds[6]->on();
  } else {
    leds[6]->off();
  }
  if (params[1]) {
    leds[7]->on();
  } else {
    leds[7]->off();
  }
}
