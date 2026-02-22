#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led
{
public:
    Led(uint8_t pin, unsigned long pulseMs = 250);

    void turnOn();
    void turnOff();
    void pulse();
    void update();

private:
    uint8_t ledPin;
    unsigned long pulseDurationMs;
    bool isPulsing;
    unsigned long lastPulseMillis;
};

#endif
