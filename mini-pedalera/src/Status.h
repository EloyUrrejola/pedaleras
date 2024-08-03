#ifndef STATUS_H
#define STATUS_H

#include "config.h"

class Status
{
  public:
    static int setParameter(uint8_t cc, bool state);
    static const bool* getAll(); 
    
  private:
    static bool chord_mode;
    static bool bass;
    static bool octave;
    static bool guitar;
    static bool reverb;
    static bool midi;
    static bool chorus;
    static bool player_1;
    static bool chord_A;
    static bool chord_B;
    static bool chord_C;
    static bool chord_D;

    struct Mapping {
        uint8_t cc;
        bool* state;
    };

    static const Mapping mapping[];

    static const uint8_t NUM_PARAMS;
};

#endif
