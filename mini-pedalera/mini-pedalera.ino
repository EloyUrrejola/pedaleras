#ifndef MINI_PEDALERA_INO
#define MINI_PEDALERA_INO

#include <ILI9488_t3.h>
#include <SPI.h>

#include "src/config.h"
#include "src/Button.h"
#include "src/Led.h"
#include "src/MidiMessage.h"
#include "src/MyClock.h"
#include "src/Screen.h"
#include "src/Settings.h"
#include "src/SongSelector.h"
#include "src/SysExMessage.h"
#include "src/Tuner.h"

const uint8_t NUMBER_OF_BUTTONS = 9;
const uint8_t NUMBER_OF_LEDS = 9;

Button *buttons[NUMBER_OF_BUTTONS];
Led *leds[NUMBER_OF_LEDS];

MidiMessage midi_message;
SysExMessage sysex_message;
SongSelector song_selector;
Settings settings;
Tuner tuner;
MyClock my_clock;

uint8_t action;

#define TFT_RST 8
#define TFT_DC  9
#define TFT_CS  10
ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST);

Screen screen(&tft);

void setup()
{
  Serial.begin(9600);

  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    uint8_t button_ccs_array[2] = {BUTTON_PIN_CCS[0][i], BUTTON_PIN_CCS[1][i]};
    buttons[i] = new Button(
      button_pins[i],
      button_ccs_array,
      button_release_ccs[i],
      button_momentary_set[i],
      button_momentary_ccs[i],
      button_push_actions[i],
      button_hold_actions[i],
      settings_buttons[i]
    );
  }
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i] = new Led(led_pins[i], led_ccs[i]);
  }

  midi_message.init(&screen, buttons, NUMBER_OF_BUTTONS, leds, NUMBER_OF_LEDS);
  sysex_message.init(&screen);
  song_selector.init(&screen, buttons, NUMBER_OF_BUTTONS, leds, NUMBER_OF_LEDS);
  settings.init(&screen, buttons, NUMBER_OF_BUTTONS, leds, NUMBER_OF_LEDS);
  tuner.init(&screen, buttons, NUMBER_OF_BUTTONS, leds, NUMBER_OF_LEDS);
  my_clock.init(&screen);

  usbMIDI.setHandleControlChange(receiveMidiMessage);
  usbMIDI.setHandleSystemExclusive(receiveSysEx);

  screen.begin();
  start();
}

void start()
{
  screen.clean();
  screen.writeMessage("READY", "");
  flash_leds(LED_FLASHING_TIMES);
  // Hay que dar tiempo a que se GP conecte con el puerto USB.
  // Habrá que hacer un ping antes.
  delay(1000);
  requestSetlist();
}

void loop()
{
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    action = buttons[i]->changed();
    if (action > 0) {
      if (action == SETTINGS_ACTION) {
        settingsMode();
      }
      if (action == SONG_SELECTOR_ACTION) {
        songSelectorMode();
      }
      if (action == TUNER_ACTION) {
        tunerMode();
      }
      if (action == CLOCK_ACTION) {
        showClock(2);
        exitClockMode();
      }
      if (action == NEXT_SONG_ACTION) {
        nextSong();
      }
      if (action == STOP_ACTION) {
        stopAudio();
      }
    }
  }
  usbMIDI.read();
}

void flash_leds(int times)
{
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, times);
  }
  leds_flash_update();
}

void leds_flash_update()
{
  bool leds_flashing[] = {true,true,true,true,true,true,true,true,true,true,true,true,true,true};
  while (any_led_flashing(leds_flashing)) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
      leds_flashing[i] = leds[i]->flashUpdateTimes();
    }
  }
}

bool any_led_flashing(bool leds_flashing[])
{
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    if (leds_flashing[i]) {
      return true;
    }
  }
  return false;
}

void receiveMidiMessage(uint8_t channel, uint8_t control, uint8_t value)
{
  midi_message.process(channel, control, value);
}

void receiveSysEx(uint8_t *data, unsigned int length)
{
  sysex_message.process(data, length);
}

void requestSetlist()
{
  usbMIDI.sendControlChange(25, 127, 4);
}

void nextSong()
{
  usbMIDI.sendControlChange(NEXT_SONT_CC, 127, 1);
}

void stopAudio()
{
  usbMIDI.sendControlChange(STOP_BUTTON_CC, 127, 1);
}

int getDatetime(char* message)
{
  int datetime = 0;
  char* delimiter = strchr(message, ':');
  if (delimiter != NULL) {
    char* datetime_str = delimiter + 1;
    datetime = atoi(datetime_str);
  }
  return datetime;
}

void settingsMode()
{
  settings.startSettingsMode();
  settings.settingsMode();
  settings.exitSettingsMode();
  exitSettingsMode();
}

void exitSettingsMode()
{
  screen.clean();
  screen.writeSongAndPart();
}

void songSelectorMode()
{
  if (SongList::getNumberOfSongs() > 0) {
    song_selector.startSongSelectorMode();
    song_selector.songSelectorMode();
    song_selector.exitSongSelectorMode();
    exitSongSelectorMode();
  } else {
    requestSetlist();
  }
}

void exitSongSelectorMode()
{
  screen.clean();
  screen.writeSongAndPart();
}

void tunerMode()
{
  tuner.startTunerMode();
  tuner.tunerMode();
  tuner.exitTunerMode();
  exitTunerMode();
}

void exitTunerMode()
{
  screen.clean();
  screen.writeSongAndPart();
}

void showClock(uint8_t wait_seconds)
{
  my_clock.startClockMode();
  my_clock.clockMode(wait_seconds);
}

void exitClockMode()
{
  screen.clean();
  screen.writeSongAndPart();
}

#endif
