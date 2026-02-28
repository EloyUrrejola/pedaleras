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
    analogWrite(ledPin, LED_ON);
    isPulsing = false;
}

void Led::turnOff() {
    analogWrite(ledPin, LOW);
    isPulsing = false;
}

void Led::pulse() {
    analogWrite(ledPin, LED_ON);
    isPulsing = true;
    lastPulseMillis = millis();
}

void Led::update() {
    if (isPulsing && millis() - lastPulseMillis >= pulseDurationMs) {
        analogWrite(ledPin, LOW);
        isPulsing = false;
    }
}
