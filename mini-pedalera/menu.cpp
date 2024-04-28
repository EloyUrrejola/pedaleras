#include "menu.h"

const char* MENU[] = {
  "Led Intensity",
  "Audio Out"
};

const char* AUDIO_SUBMENU[] = {
  "Stereo/Sep",
  "MIDI Left",
  "MIDI Rigth",
  "Bass Left",
  "Bass Right"
};

const char** SUBMENUS[] = {
  0,
  AUDIO_SUBMENU,
};