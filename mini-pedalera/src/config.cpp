#include "config.h"

const uint8_t BASS_BUTTON_CC = 14;
const uint8_t OCTAVE_BUTTON_CC = 15;
const uint8_t GUITAR_BUTTON_CC = 20;
const uint8_t REVERB_BUTTON_CC = 21;
const uint8_t TUNER_BUTTON_CC = 22;
const uint8_t CHORUS_BUTTON_CC = 23;
const uint8_t TEMPO_BUTTON_CC = 24;
const uint8_t MIDI_BUTTON_CC = 25;
const uint8_t A_CHORD_BUTTON_CC = 26;
const uint8_t B_CHORD_BUTTON_CC = 27;
const uint8_t C_CHORD_BUTTON_CC = 28;
const uint8_t D_CHORD_BUTTON_CC = 29;
const uint8_t RESET_BUTTON_CC = 31;
const uint8_t PLAY_1_BUTTON_CC = 85;
const uint8_t PLAY_2_BUTTON_CC = 86;
const uint8_t STOP_BUTTON_CC = 87;
const uint8_t PAGEUP_BUTTON_CC = 88;
const uint8_t PAGEDN_BUTTON_CC = 30;
const uint8_t NEXT_SONT_CC = 91;
const uint8_t GUITAR_SWICH_CC = 89;

const uint8_t CHORD_A_LED_CC = 26;
const uint8_t OCTAVE_LED_CC = 15;
const uint8_t GUITAR_LED_CC = 20;
const uint8_t REVERB_LED_CC = 21;
const uint8_t PAGEDN_LED_CC = 30;
const uint8_t RESET_LED_CC = 31;
const uint8_t PLAY_1_LED_CC = 85;
const uint8_t BASS_LED_CC = 14;
const uint8_t PAGEUP_LED_CC = 88;
const uint8_t CHORD_B_LED_CC = 27;
const uint8_t CHORD_C_LED_CC = 28;
const uint8_t CHORD_D_LED_CC = 29;

const uint8_t SETTINGS_ACTION = 1;
const uint8_t SONG_SELECTOR_ACTION = 2;
const uint8_t TUNER_ACTION = 3;
const uint8_t CLOCK_ACTION = 4;
const uint8_t NEXT_SONG_ACTION = 5;
const uint8_t MODE_ACTION = 6;
const uint8_t STOP_ACTION = 7;

const uint8_t BUTTON_PIN_CCS[2][9] = {
  {
    A_CHORD_BUTTON_CC, OCTAVE_BUTTON_CC, GUITAR_BUTTON_CC, REVERB_BUTTON_CC, PAGEDN_BUTTON_CC,
    RESET_BUTTON_CC, PLAY_1_BUTTON_CC, BASS_BUTTON_CC, 0
  },
  {
    A_CHORD_BUTTON_CC, B_CHORD_BUTTON_CC, C_CHORD_BUTTON_CC, D_CHORD_BUTTON_CC, PAGEDN_BUTTON_CC,
    RESET_BUTTON_CC, PLAY_1_BUTTON_CC, BASS_BUTTON_CC, 0
  }
};

const uint8_t button_pins[]            = {38,34,35,39,40,23,22,20,19};
// CCs al soltar el botón
const uint8_t button_release_ccs[]     = { 0, 0, 0, 0, 0, 0, 0, 0, PAGEUP_BUTTON_CC};
// CCs recibidos para activar momentary_ccs, por ej. guitar MOD (89)
const uint8_t button_momentary_set[]   = { 0, 0, 0, GUITAR_SWICH_CC, 0, 0, 0, 0, 0};
// Acción activada con button_momentary_set, por ej. guitar/bass (20)
const uint8_t button_momentary_ccs[]   = { 0, 0, 0, GUITAR_BUTTON_CC, 0, 0, 0, 0, 0};
// Tunner: tuner_mode (3)
const uint8_t button_push_actions[]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
// Acciones pasado un intervalo de tiempo (300 ms)
const uint8_t button_hold_actions[]    = { 0, 0, 0, 0, NEXT_SONG_ACTION, SETTINGS_ACTION, STOP_ACTION, TUNER_ACTION, SONG_SELECTOR_ACTION};
// Botones usados para los settings
const uint8_t settings_buttons[]       = { 0, 0, 5, 6, 3, 8, 0, 1, 2};

const uint8_t led_pins[]    = { 0, 1, 2, 3, 4, 5, 6, 7,24};
const uint8_t led_ccs[]     = {CHORD_A_LED_CC, OCTAVE_LED_CC, GUITAR_LED_CC, REVERB_LED_CC, PAGEDN_LED_CC, RESET_LED_CC, PLAY_1_LED_CC, BASS_LED_CC, PAGEUP_LED_CC};
// 14,15,20,21,22,23,24,25,  26,27,28,29,85,86

const int LED_FLASHING_ON  = 500;
const int LED_FLASHING_OFF = 500;
const int LED_FLASHING_TIMES = 3;
