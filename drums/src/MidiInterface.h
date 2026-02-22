#ifndef MIDIINTERFACE_H
#define MIDIINTERFACE_H

#include <Arduino.h>

class Sensor;
class Led;

class MidiInterface
{
public:
    static const int MIDI_CHANNEL = 6;

    MidiInterface();

    void begin();
    void registerSensors(Sensor **sensorArray, uint8_t numberOfSensors);
    void sendControlChange(uint8_t control, uint8_t value);
    void handleControlChange(uint8_t channel, uint8_t control, uint8_t value);

private:
    Sensor **sensors = nullptr;
    uint8_t sensorCount = 0;
};

#endif
