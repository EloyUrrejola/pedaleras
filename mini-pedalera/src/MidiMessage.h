#ifndef MIDI_MESSAGE_H
#define MIDI_MESSAGE_H

#include <Arduino.h>

#include "Button.h"
#include "Led.h"
#include "Settings.h"

class MidiMessage
{
  public:
    MidiMessage();
    void init(Button* buttons[], uint8_t number_of_buttons, Led* leds[], uint8_t number_of_leds);
    void process(uint8_t channel, uint8_t control, uint8_t value);
    
  private:
    Button** buttons;
    Led** leds;
    uint8_t number_of_buttons;
    uint8_t number_of_leds;
    const uint8_t LEDS_MIDI_CHANNEL = 2;
    const uint8_t BUTTON_MODE_MIDI_CHANNEL = 3;
    const uint8_t SETTING_CHANNEL = 4;

    bool isLedMessage(uint8_t channel);
    bool isButtonModeMessage(uint8_t channel);
    bool isSettingMessage(uint8_t channel);
    void processLedMessage(uint8_t cc, uint8_t value);
    void processButtonModeMessage(uint8_t cc, uint8_t value);
    void processSettingMessage(uint8_t cc, uint8_t value);
    int getLedIndexByCc(uint8_t cc);
    int getButtonIndexBySetMomentaryCc(uint8_t cc);
};

#endif