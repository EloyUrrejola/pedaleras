#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

extern const uint8_t BASS_BUTTON_CC;
extern const uint8_t OCTAVE_BUTTON_CC;
extern const uint8_t GUITAR_BUTTON_CC;
extern const uint8_t REVERB_BUTTON_CC;
extern const uint8_t TUNER_BUTTON_CC;
extern const uint8_t CHORUS_BUTTON_CC;
extern const uint8_t TEMPO_BUTTON_CC;
extern const uint8_t MIDI_BUTTON_CC;
extern const uint8_t A_CHORD_BUTTON_CC;
extern const uint8_t B_CHORD_BUTTON_CC;
extern const uint8_t C_CHORD_BUTTON_CC;
extern const uint8_t D_CHORD_BUTTON_CC;
extern const uint8_t RESET_BUTTON_CC;
extern const uint8_t PLAY_1_BUTTON_CC;
extern const uint8_t PLAY_2_BUTTON_CC;
extern const uint8_t STOP_BUTTON_CC;
extern const uint8_t PAGEUP_BUTTON_CC;
extern const uint8_t PAGEDN_BUTTON_CC;
extern const uint8_t NEXT_SONT_CC;
extern const uint8_t GUITAR_SWICH_CC;
extern const uint8_t MODE_CHANGE_CC;

extern const uint8_t CHORD_A_LED_CC;
extern const uint8_t OCTAVE_LED_CC;
extern const uint8_t GUITAR_LED_CC;
extern const uint8_t REVERB_LED_CC;
extern const uint8_t CHORUS_LED_CC;
extern const uint8_t PAGEDN_LED_CC;
extern const uint8_t RESET_LED_CC;
extern const uint8_t PLAY_1_LED_CC;
extern const uint8_t BASS_LED_CC;
extern const uint8_t PAGEUP_LED_CC;
extern const uint8_t CHORD_B_LED_CC;
extern const uint8_t CHORD_C_LED_CC;
extern const uint8_t CHORD_D_LED_CC;

extern const uint8_t LEDS_MIDI_CHANNEL;
extern const uint8_t BUTTON_MODE_MIDI_CHANNEL;
extern const uint8_t SETTING_CHANNEL;
extern const uint8_t MODE_CHANNEL;

extern const uint8_t SETTINGS_ACTION;
extern const uint8_t SONG_SELECTOR_ACTION;
extern const uint8_t TUNER_ACTION;
extern const uint8_t CLOCK_ACTION;
extern const uint8_t NEXT_SONG_ACTION;
extern const uint8_t MODE_ACTION;
extern const uint8_t STOP_ACTION;

constexpr uint8_t NUMBER_OF_BUTTONS = 9;
extern const uint8_t BUTTON_PIN_CCS[2][NUMBER_OF_BUTTONS];

extern const uint8_t button_pins[];
// CCs al soltar el botón
extern const uint8_t button_release_ccs[];
// CCs recibidos para activar momentary_ccs, por ej. guitar MOD (89)
extern const uint8_t button_momentary_set[];
// Acción activada con button_momentary_set, por ej. guitar/bass (20)
extern const uint8_t button_momentary_ccs[];
// Tunner: tuner_mode (3)
extern const uint8_t button_push_actions[];
// Acciones pasado un intervalo de tiempo (300 ms)
extern const uint8_t button_hold_actions[];
// Tiempo de pulsado de cada botón en ms
extern const int     button_hold_times[];
// Botones usados para los settings
extern const uint8_t settings_buttons[];

constexpr uint8_t NUMBER_OF_LEDS = 9;
extern const uint8_t led_ccs[2][NUMBER_OF_LEDS];
extern const uint8_t LED_CCS[2][NUMBER_OF_LEDS];
extern const uint8_t led_pins[];

extern const int LED_FLASHING_ON;
extern const int LED_FLASHING_OFF;
extern const int LED_FLASHING_TIMES;

#endif