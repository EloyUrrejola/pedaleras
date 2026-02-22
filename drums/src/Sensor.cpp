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
    threshold = map(value, 0, 127, 0, 1023);
    float h = map(value, 0, 127, 0, 30) / 100.0;
    hysteresisAmount = threshold * h;
    Serial.print("threshold: ");
    Serial.println(threshold);
    Serial.print("hysteresisAmount: ");
    Serial.println(hysteresisAmount);
}
void Sensor::setHysteresis(uint8_t value) {
    float h = map(value, 0, 127, 0, 30) / 100.0;
    hysteresisAmount = threshold * h;
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

void Sensor::setChordState(bool chord) {
    chordState = chord;
    if (ledChord) {
        if (chord) {
            ledChord->turnOn();
        } else {
            ledChord->turnOff();
        }
    }
}

void Sensor::update() {
    int sensorValue = analogRead(sensorPin);
    bool isAbove = !wasAbove ? sensorValue >= threshold : sensorValue >= (threshold - hysteresisAmount);

    if (isAbove && !wasAbove && millis() - lastTrigger > retriggerIntervalMs) {
        sendTrigger();
        lastTrigger = millis();

        if (ledBounce) {
            ledBounce->pulse();
        }
        if (ledCcSend) {
            ledCcSend->pulse();
        }
    }

    wasAbove = isAbove;

    if (ledBounce) {
        ledBounce->update();
    }
    if (ledCcSend) {
        ledCcSend->update();
    }
}

void Sensor::sendTrigger() {
    if (midi) {
        midi->sendControlChange(triggerCc, topVelocity);
    }
}
