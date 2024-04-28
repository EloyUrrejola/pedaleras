#ifndef MINI_PEDALERA_INO
#define MINI_PEDALERA_INO

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#include "Button.h"
#include "MyClock.h"
#include "Led.h"
#include "MidiMessage.h"
#include "Screen.h"
#include "Settings.h"
#include "SongSelector.h"
#include "SysExMessage.h"
#include "Tuner.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128

//const uint8_t   OLED_pin_scl_sck        = 13;
//const uint8_t   OLED_pin_sda_mosi       = 11;
const uint8_t  OLED_pin_cs_ss          = 10;
const uint8_t  OLED_pin_res_rst        = 8;
const uint8_t  OLED_pin_dc_rs          = 9;

const uint8_t button_pins[]            = {27,30,31,32,16,17, 3, 2,  38,34,35,39,40,23,22,21,20,19};
const uint8_t button_ccs[]             = {14,15,20,21,22,23,24,25,  26,27,28,29,30,31,85,86,87, 0};
// CCs al soltar el botón
const uint8_t button_release_ccs[]     = { 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0,88};
// CCs recibidos para activar momentary_ccs, por ej. guitar MOD (89)
const uint8_t button_momentary_set[]   = { 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0,89, 0, 0, 0, 0, 0, 0};
// Acción activada con button_momentary_set, por ej. guitar/bass (20)
const uint8_t button_momentary_ccs[]   = { 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0,20, 0, 0, 0, 0, 0, 0};
// Tunner: tuner_mode (3)
const uint8_t button_push_actions[]    = { 0, 0, 0, 0, 3, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// Acciones pasado un intervalo de tiempo (300 ms)
const uint8_t button_hold_actions[]    = { 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 5, 4, 0, 0, 1, 2};
// Botones usados para los settings
const uint8_t settings_buttons[]       = { 0, 0, 0, 0, 7, 0, 0, 0,   0, 0, 5, 6, 3, 8, 0, 4, 1, 2};
const uint8_t SETTINGS_ACTION = 1;
const uint8_t SONG_SELECTOR_ACTION = 2;
const uint8_t TUNER_ACTION = 3;
const uint8_t CLOCK_ACTION = 4;
const uint8_t NEXT_SONG_ACTION = 5;
const uint8_t NUMBER_OF_BUTTONS = sizeof(button_pins) / sizeof(button_pins[0]);

const uint8_t led_pins[]    = { 7, 4, 5, 6,24,25,28,29,  33,37,36,14,18,15};
const uint8_t led_ccs[]     = {14,15,20,21,22,23,24,25,  26,27,28,29,85,86};
const uint8_t NUMBER_OF_LEDS = sizeof(led_pins) / sizeof(led_pins[0]);

Button *buttons[NUMBER_OF_BUTTONS];
Led *leds[NUMBER_OF_LEDS];
MidiMessage midi_message;
SysExMessage sysex_message;
SongSelector song_selector;
Settings settings;
Tuner tuner;
MyClock my_clock;

const int LED_FLASHING_ON  = 500;
const int LED_FLASHING_OFF = 500;
const int LED_FLASHING_TIMES = 3;

uint8_t action;

Adafruit_SSD1351 adafruit = Adafruit_SSD1351(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &SPI,
  OLED_pin_cs_ss,
  OLED_pin_dc_rs,
  OLED_pin_res_rst
);
Screen screen(&adafruit);

void setup()
{
  Serial.begin(9600);

  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    buttons[i] = new Button(
      button_pins[i],
      button_ccs[i],
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

  midi_message.init(buttons, NUMBER_OF_BUTTONS, leds, NUMBER_OF_LEDS);
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
  screen.writeMessage("", "READY");
  flash_leds(LED_FLASHING_TIMES);
  // Hay que dar tiempo a que se GP conecte con el puerto USB.
  // Habrá que hacer un ping antes.
  delay(3000);
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
  usbMIDI.sendControlChange(91, 127, 1);
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
