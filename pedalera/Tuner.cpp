#include "Tuner.h"

Tuner::Tuner()
{
}

void Tuner::init(Screen *screen, Button **buttons, uint8_t number_of_buttons, Led **leds, uint8_t number_of_leds)
{
  this->screen = screen;
  this->buttons = buttons;
  this->number_of_buttons = number_of_buttons;
  this->leds = leds;
  this->number_of_leds = number_of_leds;
}

void Tuner::startTunerMode()
{
  screen->clean();
  //screen->writeTunerTitle();
  screen->showTuningBackground();
}

void Tuner::tunerMode()
{
  bool tuner_mode = true;
  uint8_t cc;
  uint8_t value;
  char note[3];
  uint8_t last_tuning = 0;
  while (tuner_mode) {
    for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
      leds[_leds_flashing_index[i]]->flashUpdate();
    }
    for (uint8_t i = 0; i < number_of_buttons; i++) {
      uint8_t action = buttons[i]->settingsChanged();
      if (action == _exit) {
        buttons[i]->sendControlChange(buttons[i]->getButtonCc());
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
        showTuning(value, last_tuning);
        last_tuning = value;
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
  
}