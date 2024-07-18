#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led
{
  public:
    Led(uint8_t, uint8_t[2]);
    void on();
    void off();
    void flash(int time_on, int time_off, int times);
    void flashUpdate();
    bool flashUpdateTimes();
    static uint8_t getLedIntensityLevel();
    static void setLedIntensityLevel(uint8_t intensity_level);
    uint8_t getLedCc();
    static void updateLedsMode(uint8_t);
    
  private:
    const bool ON = true;
    const bool OFF = false;
    static const uint8_t NUMBER_OF_LEVELS = 3;
    static const uint8_t INTENSITIES[];
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
};

#endif