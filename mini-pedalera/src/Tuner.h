#ifndef TUNER_H
#define TUNER_H

#include <Arduino.h>

#include "Screen.h"
#include "Button.h"
#include "Led.h"

class Tuner
{
  public:
    Tuner();
    void init(Screen *screen, Button **buttons, uint8_t number_of_buttons, Led **leds, uint8_t number_of_leds);
    void startTunerMode();
    void tunerMode();
    void exitTunerMode();
  private:
    Screen *screen;
    Button **buttons;
    Led    **leds;
    uint8_t number_of_buttons;
    uint8_t number_of_leds;

    const uint8_t MIDI_CHANNEL = 3;
    const char *NOTES[12] = {"C","Db","D","Eb","E","F","F#","G","Ab","A","Bb","B"};
    const int LED_FLASHING_ON  = 500;
    const int LED_FLASHING_OFF = 500;
    const uint8_t _number_of_leds_flashing = 1;
    const uint8_t _leds_flashing_index[1] = {4};
    const uint8_t _number_of_leds_tunning = 8;
    const uint8_t _leds_tuning_index[8] = {0,1,2,3,8,9,10,11};
    const uint8_t _exit = 7;

    void showTuning(uint8_t tuning, uint8_t last_tuning);
    void showLedTuning(uint8_t tuning);
    void getNote(uint8_t value, char* note);
};

#endif