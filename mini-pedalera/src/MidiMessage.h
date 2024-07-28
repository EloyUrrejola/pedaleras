#ifndef MIDI_MESSAGE_H
#define MIDI_MESSAGE_H

#include <Arduino.h>

#include "config.h"
#include "Button.h"
#include "Led.h"
#include "Settings.h"
#include "Status.h"

class MidiMessage
{
  public:
    MidiMessage();
    void init(Screen *screen, Button* buttons[], uint8_t number_of_buttons);
    void process(uint8_t channel, uint8_t control, uint8_t value);
    
  private:
    Screen *screen;
    Button** buttons;
    uint8_t number_of_buttons;
    

    bool isLedMessage(uint8_t channel);
    bool isButtonModeMessage(uint8_t channel);
    bool isSettingMessage(uint8_t channel);
    void processLedMessage(uint8_t cc, uint8_t value);
    void processButtonModeMessage(uint8_t cc, uint8_t value);
    void processSettingMessage(uint8_t cc, uint8_t value);
    void processModeMessage(uint8_t cc, uint8_t value);
    int getLedIndexByCc(uint8_t cc);
    int getButtonIndexBySetMomentaryCc(uint8_t cc);
    bool valueToState(uint8_t value);
};

#endif