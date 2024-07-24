#include "Status.h"

bool Status::chord_mode = false;
bool Status::bass = false;
bool Status::octave = false;
bool Status::guitar = false;
bool Status::reverb = false;
bool Status::chorus = false;
bool Status::player_1 = false;
bool Status::chord_A = false;
bool Status::chord_B = false;
bool Status::chord_C = false;
bool Status::chord_D = false;

const Status::Mapping Status::mapping[] = {
    {MODE_CHANGE_CC, &Status::chord_mode},
    {BASS_LED_CC, &Status::bass},
    {OCTAVE_LED_CC, &Status::octave},
    {GUITAR_LED_CC, &Status::guitar},
    {REVERB_LED_CC, &Status::reverb},
    {CHORUS_LED_CC, &Status::chorus},
    {PLAY_1_LED_CC, &Status::player_1},
    {CHORD_A_LED_CC, &Status::chord_A},
    {CHORD_B_LED_CC, &Status::chord_B},
    {CHORD_C_LED_CC, &Status::chord_C},
    {CHORD_D_LED_CC, &Status::chord_D}
};

const uint8_t Status::NUM_PARAMS = sizeof(Status::mapping) / sizeof(Status::mapping[0]);

void Status::setParameter(uint8_t cc, bool status)
{
  for (uint8_t i = 0; i < NUM_PARAMS; ++i) {
    if (mapping[i].cc == cc) {
        *(mapping[i].status) = status;
        break;
    }
  }
}

const bool* Status::getAll()
{
  static bool params[11];
  params[0] = chord_mode;
  params[1] = bass;
  params[2] = octave;
  params[3] = guitar;
  params[4] = reverb;
  params[5] = chorus;
  params[6] = player_1;
  params[7] = chord_A;
  params[8] = chord_B;
  params[9] = chord_C;
  params[10] = chord_D;
  
  return params;
}
