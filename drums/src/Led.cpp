#include "Led.h"

Led::Led(uint8_t pin, unsigned long pulseMs) {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    pulseDurationMs = pulseMs;
    isPulsing = false;
    lastPulseMillis = 0;
}

void Led::turnOn() {
    digitalWrite(ledPin, HIGH);
    isPulsing = false;
}

void Led::turnOff() {
    digitalWrite(ledPin, LOW);
    isPulsing = false;
}

void Led::pulse() {
    pinMode(13, OUTPUT);
    digitalWrite(ledPin, HIGH);
    isPulsing = true;
    lastPulseMillis = millis();
}

void Led::update() {
    if (isPulsing && millis() - lastPulseMillis >= pulseDurationMs) {
        digitalWrite(ledPin, LOW);
        isPulsing = false;
    }
}
