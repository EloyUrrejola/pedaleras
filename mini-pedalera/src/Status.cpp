#include "Status.h"

bool Status::chord_mode = false;
bool Status::bass = false;
bool Status::octave = false;
bool Status::guitar = false;
bool Status::reverb = false;
bool Status::midi = false;
bool Status::chorus = false;
bool Status::player_1 = false;
bool Status::chord_A = false;
bool Status::chord_B = false;
bool Status::chord_C = false;
bool Status::chord_D = false;
bool Status::instrument = false;

const Status::Mapping Status::mapping[] = {
    {MODE_CHANGE_CC, &Status::chord_mode},
    {BASS_LED_CC, &Status::bass},
    {OCTAVE_LED_CC, &Status::octave},
    {GUITAR_LED_CC, &Status::guitar},
    {REVERB_LED_CC, &Status::reverb},
    {MIDI_LED_CC, &Status::midi},
    {CHORUS_LED_CC, &Status::chorus},
    {PLAY_1_LED_CC, &Status::player_1},
    {CHORD_A_LED_CC, &Status::chord_A},
    {CHORD_B_LED_CC, &Status::chord_B},
    {CHORD_C_LED_CC, &Status::chord_C},
    {CHORD_D_LED_CC, &Status::chord_D},
    {INSTRUMENT_CHANGE_CC, &Status::instrument}
};

const uint8_t Status::NUM_PARAMS = sizeof(Status::mapping) / sizeof(Status::mapping[0]);

int Status::setParameter(uint8_t cc, bool state)
{
  for (uint8_t i = 0; i < NUM_PARAMS; ++i) {
    if (mapping[i].cc == cc) {
        *(mapping[i].state) = state;
        return i;
    }
  }
  return -1;
}

const bool* Status::getAll()
{
  static bool params[13];
  params[0] = chord_mode;
  params[1] = bass;
  params[2] = octave;
  params[3] = guitar;
  params[4] = reverb;
  params[5] = midi;
  params[6] = chorus;
  params[7] = player_1;
  params[8] = chord_A;
  params[9] = chord_B;
  params[10] = chord_C;
  params[11] = chord_D;
  params[12] = instrument;
  
  return params;
}

const int Status::getMode()
{
  return chord_mode ? 1 : 0;
}

bool Status::isBassActive()
{
  return bass || guitar;
}

const char* Status::getInstrument()
{
  if (instrument) {
    return "Status";
  }
  return "Warwick";
}
