#include "Sensor.h"

Sensor::Sensor(
    uint8_t sensorPin,
    uint8_t triggerCc,
    uint8_t thresholdCc,
    uint8_t hysteresisCc,
    uint8_t retriggerCc,
    uint8_t readyStateCc,
    uint8_t chordStateCc
) {
    this->sensorPin = sensorPin;
    this->triggerCc = triggerCc;
    this->thresholdCc = thresholdCc;
    this->hysteresisCc = hysteresisCc;
    this->retriggerCc = retriggerCc;
    this->readyStateCc = readyStateCc;
    this->chordStateCc = chordStateCc;

    threshold = DEFAULT_THRESHOLD;
    hysteresisAmount = DEFAULT_HYSTERESIS;
    retriggerIntervalMs = DEFAULT_RETRIGGER_MS;

    lastTrigger = 0;
}

void Sensor::setThreshold(uint8_t value) {
    threshold = map(value, 0, 127, 1, 1023);
    Serial.print("threshold: ");
    Serial.println(threshold);
}
void Sensor::setHysteresis(uint8_t value) {
    hysteresisAmount = map(value, 0, 127, 0, 1023);
    Serial.print("hysteresisAmount: ");
    Serial.println(hysteresisAmount);
}
void Sensor::setRetriggerMs(uint8_t value) {
    retriggerIntervalMs = map(value, 0, 127, 0, 250);
    Serial.print("retriggerIntervalMs: ");
    Serial.println(retriggerIntervalMs);
}
void Sensor::setMidiInterface(MidiInterface *midiInterface) { midi = midiInterface; }

void Sensor::setReadyState(bool ready) {
    readyState = ready;
    if (ledReady) {
        if (ready) {
            ledReady->turnOn();
        } else {
            ledReady->turnOff();
        }
    }
}

void Sensor::setChordLeds(Led** leds, uint8_t count) {
    chordLedCount = (count > MAX_CHORD_LEDS) ? MAX_CHORD_LEDS : count;

    for (uint8_t i = 0; i < chordLedCount; i++) {
        chordLeds[i] = leds[i];
    }
}

void Sensor::setChordState(uint8_t value) {
    if (chordLedCount == 0) {
        return;
    }

    // Apagar todos
    for (uint8_t i = 0; i < chordLedCount; i++) {
        if (chordLeds[i]) {
            chordLeds[i]->turnOff();
        }
    }

    // Encender el seleccionado
    if (value > 0) {
        uint8_t index = (value - 1) % chordLedCount;
        if (chordLeds[index]) {
            chordLeds[index]->turnOn();
        }
    }
}

void Sensor::update() {
    int sensorValue = analogRead(sensorPin);
    bool isAbove = !wasAbove ? sensorValue >= threshold : sensorValue >= (threshold - hysteresisAmount);

    if (isAbove && !wasAbove && millis() - lastTrigger > retriggerIntervalMs) {
        sendTrigger();
        lastTrigger = millis();

        if (ledCcSend) {
            ledCcSend->pulse();
        }
    }

    wasAbove = isAbove;

    if (ledCcSend) {
        ledCcSend->update();
    }
}

void Sensor::sendTrigger() {
    if (midi) {
        midi->sendControlChange(triggerCc, topVelocity);
    }
}
