#include "Tuner.h"

Tuner::Tuner()
{
}

void Tuner::init(Screen *screen, Button **buttons, uint8_t number_of_buttons)
{
  this->screen = screen;
  this->buttons = buttons;
  this->number_of_buttons = number_of_buttons;
}

void Tuner::startTunerMode()
{
  screen->clean();
  screen->writeSectionTitle(tuner_title);

  usbMIDI.sendControlChange(TUNER_BUTTON_CC, 127, 1);
  screen->showTuningBackground();
}

void Tuner::tunerMode()
{
  bool tuner_mode = true;
  uint8_t cc;
  uint8_t value;
  char note[3];
  int8_t last_tuning = 0;
  int8_t tuning;
  while (tuner_mode) {
    Led::updateFlashingLeds(_leds_flashing_index, _number_of_leds_flashing);
    for (uint8_t i = 0; i < number_of_buttons; i++) {
      uint8_t action = buttons[i]->settingsChanged();
      if (action == _exit) {
        usbMIDI.sendControlChange(TUNER_BUTTON_CC, 0, 1);
        tuner_mode = false;
      }
    }
    if (usbMIDI.read(MIDI_CHANNEL)) {
      //showLedTuning(usbMIDI.getData1(), usbMIDI.getData2());
      cc = usbMIDI.getData1();
      value = usbMIDI.getData2();
      if (cc == 20) {
        getNote(value, note);
        screen->showNote(note);
      }
      if (cc == 21) {
        tuning = value - 63;
        showTuning(tuning, last_tuning);
        last_tuning = tuning;
      }
    }
  }
}

void Tuner::showTuning(uint8_t tuning, uint8_t last_tuning)
{
  screen->showTuning(tuning, last_tuning);
  //showLedTuning(note, tuning);
}

void Tuner::getNote(uint8_t value, char* note)
{
  strcpy(note, NOTES[value % 12]);
}

void Tuner::showLedTuning(uint8_t tuning)
{
  
}

void Tuner::exitTunerMode()
{
  Led::setStatusLeds();
}