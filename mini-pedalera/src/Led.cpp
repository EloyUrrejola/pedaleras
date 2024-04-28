#include "Led.h"

uint8_t Led::led_intensity_level = 1;
const uint8_t Led::INTENSITIES[NUMBER_OF_LEVELS] = {5, 20, 127};

Led::Led(uint8_t pin, uint8_t cc)
{
  pinMode(pin, OUTPUT);
  led_pin = pin;
  led_cc = cc;
}

void Led::on()
{
  analogWrite(led_pin, Led::INTENSITIES[led_intensity_level]);
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
  return Led::led_intensity_level + 1;
}

void Led::setLedIntensityLevel(uint8_t intensity_level)
{
  Led::led_intensity_level = intensity_level - 1;
}

uint8_t Led::getLedCc()
{
  return led_cc;
}
