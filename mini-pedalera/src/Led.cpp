#include "Led.h"
#include "Status.h"

uint8_t Led::leds_mode = BASS_MODE;
uint8_t Led::led_intensity_level = 5;
Led* Led::leds[number_of_leds] = { nullptr };

void Led::init()
{
  for (uint8_t i = 0; i < number_of_leds; i++) {
    uint8_t led_ccs_array[2] = {LED_CCS[0][i], LED_CCS[1][i]};
    leds[i] = new Led(led_pins[i], led_ccs_array);
  }
}

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

void Led::startFlashingLeds(const uint8_t leds_index[], const uint8_t number_of_leds_flashing)
{
  for (uint8_t i = 0; i < number_of_leds_flashing; i++) {
    leds[leds_index[i]]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, -1);
  }
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

void Led::updateFlashingLeds(const uint8_t leds_index[], const uint8_t number_of_leds_flashing)
{
  for (uint8_t i = 0; i < number_of_leds_flashing; i++) {
    leds[leds_index[i]]->flashUpdate();
  }
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

void Led::flashLeds(int times)
{
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, times);
  }
  ledsFlashUpdate();
}

void Led::ledsFlashUpdate()
{
  bool leds_flashing[] = {true,true,true,true,true,true,true,true,true};
  while (anyLedFlashing(leds_flashing)) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
      leds_flashing[i] = leds[i]->flashUpdateTimes();
    }
  }
}

bool Led::anyLedFlashing(bool leds_flashing[])
{
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    if (leds_flashing[i]) {
      return true;
    }
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

void Led::setStatusLeds()
{
  const bool* params = Status::getAll();
  if (params[8]) {
    leds[0]->on();
  } else {
    leds[0]->off();
  }
  if (params[0]) {
    if (params[9]) {
      leds[1]->on();
    } else {
      leds[1]->off();
    }
    if (params[10]) {
      leds[2]->on();
    } else {
      leds[2]->off();
    }
    if (params[11]) {
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
  leds[4]->off();
  leds[5]->off();
  if (params[7]) {
    leds[6]->on();
  } else {
    leds[6]->off();
  }
  if (params[1]) {
    leds[7]->on();
  } else {
    leds[7]->off();
  }
  leds[8]->off();
}

void Led::setLedState(uint8_t cc, bool state)
{
  int led_index = getLedIndexByCc(cc);
  if (led_index > -1) {
    if (state) {
      leds[led_index]->on();
    } else {
      leds[led_index]->off();
    }
  }
}

int Led::getLedIndexByCc(uint8_t cc)
{
  for (uint8_t i = 0; i < number_of_leds; i++) {
    if (cc == leds[i]->getLedCc()) {
      return i;
    }
  }
  return -1;
}
