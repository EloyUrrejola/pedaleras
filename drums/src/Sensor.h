#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "MidiInterface.h"
#include "Led.h"

class Sensor
{
public:
    static const int DEFAULT_THRESHOLD = 160;
    static const int DEFAULT_HYSTERESIS = 30;
    static const unsigned long DEFAULT_RETRIGGER_MS = 25;
    static const uint8_t DEFAULT_TOP_VELOCITY = 127;

    Sensor(
        uint8_t sensorPin,
        uint8_t triggerCc,
        uint8_t thresholdCc,
        uint8_t histeresisCc,
        uint8_t retriggerCc,
        uint8_t readyStateCc,
        uint8_t chordStateCc
    );

    void update();
    void setThreshold(uint8_t value);
    void setHysteresis(uint8_t value);
    void setRetriggerMs(uint8_t value);
    void setMidiInterface(MidiInterface *midiInterface);

    void setReadyState(bool ready);
    //void setChordState(bool chord);

    void setChordLeds(Led** leds, uint8_t count);
    void setChordState(uint8_t value);

    uint8_t triggerCc;
    uint8_t thresholdCc;
    uint8_t hysteresisCc;
    uint8_t retriggerCc;
    uint8_t readyStateCc;
    uint8_t chordStateCc;

    Led *ledReady = nullptr;
    Led *ledCcSend = nullptr;

    static const uint8_t MAX_CHORD_LEDS = 4;
    Led* chordLeds[MAX_CHORD_LEDS] = { nullptr };
    uint8_t chordLedCount = 0;

private:
    void sendTrigger();

    unsigned long lastTrigger;
    bool wasAbove = false;

    uint8_t sensorPin;
    int threshold;
    int hysteresisAmount;
    unsigned long retriggerIntervalMs;
    const uint8_t topVelocity = DEFAULT_TOP_VELOCITY;

    bool readyState = false;

    MidiInterface *midi = nullptr;
};

#endif
