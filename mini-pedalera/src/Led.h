#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "config.h"

class Led
{
  public:
    static void init();
    Led(uint8_t, uint8_t[2]);
    void on();
    void off();
    static void startFlashingLeds(const uint8_t leds_index[], const uint8_t number_of_leds_flashing);
    void flash(int time_on, int time_off, int times);
    static void updateFlashingLeds(const uint8_t leds_index[], const uint8_t number_of_leds_flashing);
    void flashUpdate();
    bool flashUpdateTimes();
    static uint8_t getLedIntensityLevel();
    static void setLedIntensityLevel(uint8_t intensity_level);
    uint8_t getLedCc();
    static void updateLedsMode(uint8_t);
    static void setStatusLeds();
    static void setLedState(uint8_t cc, bool state);
    static void flashLeds(int times);
    
  private:
    const bool ON = true;
    const bool OFF = false;
    static const uint8_t BASS_MODE = 0;

    static uint8_t leds_mode;
    static uint8_t led_intensity_level;
    uint8_t led_pin;
    uint8_t led_ccs[2];
    unsigned int led_time_on;
    unsigned int led_time_off;
    int led_times;
    bool led_state;
    unsigned long led_time_now;
    unsigned long led_time_start;

    static const uint8_t number_of_leds = NUMBER_OF_LEDS;
    static Led* leds[number_of_leds];
    static int getLedIndexByCc(uint8_t cc);
    static void ledsFlashUpdate();
    static bool anyLedFlashing(bool leds_flashing[]);
};

#endif