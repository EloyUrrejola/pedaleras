#include "Button.h"

Button::Button(uint8_t pin, uint8_t cc, uint8_t release_cc, uint8_t set_momentary_cc, uint8_t momentary_cc, uint8_t push_action, uint8_t hold_action, uint8_t settings_action)
{
  pinMode(pin, INPUT_PULLUP);
  button_pin = pin;
  button_cc = cc;
  button_release_cc = release_cc;
  button_momentary = false;
  button_set_momentary_cc = set_momentary_cc;
  button_momentary_cc = momentary_cc;
  button_push_action = push_action;
  button_hold_action = hold_action;
  button_settings_action = settings_action;
  button_pressed = false;
  button_debouncer = new Bounce();
  button_debouncer->attach(button_pin);
  button_debouncer->interval(debouncer_interval);
}

uint8_t Button::getButtonCc()
{
  return button_cc;
}

uint8_t Button::changed()
{
  button_debouncer->update();
  if (button_debouncer->fell()) {
    if (button_momentary) {
      sendControlChange(button_momentary_cc);
    } else {
      sendControlChange(button_cc);
    }
    if (button_push_action > 0) {
      return button_push_action;
    }
    if (button_hold_action > 0) {
      button_time_start = millis();
      button_pressed = true;
    }
  }
  if (button_debouncer->rose()) {
    if (button_momentary) {
      sendControlChange(button_momentary_cc);
    } else {
      sendControlChange(button_release_cc);
    }
    if (button_hold_action > 0) {
      button_pressed = false;
    }
  }
  if (button_pressed) {
    button_time_now = millis();
    if (button_time_now - button_time_start > ACTION_TIME) {
      button_pressed = false;
      return button_hold_action;
    }
  }
  return 0;
}

void Button::sendControlChange(uint8_t cc)
{
  if (cc > 0) {
    usbMIDI.sendControlChange(cc, top_velocity, channel);
  }
}

void Button::changeMomentary(bool state)
{
  button_momentary = state;
}

uint8_t Button::settingsChanged()
{
  if (button_settings_action == 0) {
    return 0;
  }
  button_debouncer->update();
  if (isContinuous()) {
    if (button_debouncer->fell()) {
      button_time_start = millis();
      button_pressed = true;
      return button_settings_action;
    }
    if (button_pressed) {
      if (button_debouncer->rose()) {
        button_pressed = false;
        return 0;
      }
      button_time_now = millis();
      if (button_time_now - button_time_start > ACTION_TIME) {
        delay(CONTINUOUS_DELAY);
        return button_settings_action;
      }
    }
  }
  if (button_debouncer->fell()) {
    return button_settings_action;
  }
  return 0;
}

bool Button::isContinuous()
{
  if (button_settings_action == 3 || button_settings_action == 2) {
    return true;
  }
  return false;
}

uint8_t Button::getSetMomentaryCc()
{
  return button_set_momentary_cc;
}
