#include "MidiMessage.h"

MidiMessage::MidiMessage()
{
}

void MidiMessage::init(Screen *screen, Button* buttons[], uint8_t number_of_buttons)
{
  this->screen = screen;
  this->buttons = buttons;
  this->number_of_buttons = number_of_buttons;
}

void MidiMessage::process(uint8_t channel, uint8_t control, uint8_t value)
{
  if (isLedMessage(channel)) {
    processLedMessage(control, value);
  }
  if (isButtonModeMessage(channel)) {
    processButtonModeMessage(control, value);
  }
  if (isSettingMessage(channel)) {
    processSettingMessage(control, value);
  }
}

bool MidiMessage::isLedMessage(uint8_t channel)
{
  if (channel == LEDS_MIDI_CHANNEL) {
    return true;
  }
  return false;
}

bool MidiMessage::isButtonModeMessage(uint8_t channel)
{
  if (channel == BUTTON_MODE_MIDI_CHANNEL) {
    return true;
  }
  return false;
}

bool MidiMessage::isSettingMessage(uint8_t channel)
{
  if (channel == SETTING_CHANNEL) {
    return true;
  }
  return false;
}

void MidiMessage::processLedMessage(uint8_t cc, uint8_t value)
{
  bool state = valueToState(value);
  Led::setLedState(cc, state);
  
  if (cc == MODE_CHANGE_CC) {
    processModeMessage(cc, value);
  }
  int param_index = Status::setParameter(cc, state);
  if (param_index >= 0) {
    screen->writeStatusBarParameter(param_index, state);
  }
}

void MidiMessage::processButtonModeMessage(uint8_t cc, uint8_t value)
{
  int button_index = getButtonIndexBySetMomentaryCc(cc);
  if (button_index > -1) {
    buttons[button_index]->changeMomentary(valueToState(value));
  }
}

int MidiMessage::getButtonIndexBySetMomentaryCc(uint8_t cc)
{
  for (uint8_t i = 0; i < number_of_buttons; i++) {
    if (cc == buttons[i]->getSetMomentaryCc()) {
      return i;
    }
  }
  return -1;
}

void MidiMessage::processSettingMessage(uint8_t cc, uint8_t value)
{
  Settings::setSettingValue(cc, value);
}

void MidiMessage::processModeMessage(uint8_t cc, uint8_t value)
{
  uint8_t mode = (valueToState(value)) ? 1 : 0;
  Button::updateButtonsMode(mode);
  Led::updateLedsMode(mode);
}

bool MidiMessage::valueToState(uint8_t value)
{
  bool state = false;
  if (value == 127) {
    state = true;
  }
  return state;
}
