#include "MidiInterface.h"
#include "Sensor.h"

MidiInterface *midiInterfaceInstance = nullptr;

void midiControlChangeHandler(byte channel, byte control, byte value) {
    if (midiInterfaceInstance) {
        midiInterfaceInstance->handleControlChange(channel, control, value);
    }
}

MidiInterface::MidiInterface() {}

void MidiInterface::begin() {
    midiInterfaceInstance = this;
    usbMIDI.setHandleControlChange(midiControlChangeHandler);
}

void MidiInterface::registerSensors(Sensor **sensorArray, uint8_t numberOfSensors) {
    sensors = sensorArray;
    sensorCount = numberOfSensors;

    for (uint8_t i = 0; i < sensorCount; i++) {
        sensors[i]->setMidiInterface(this);
    }
}

void MidiInterface::handleControlChange(uint8_t channel, uint8_t control, uint8_t value) {
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        if (channel != MIDI_CHANNEL) {
            continue;
        }

        if (control == sensors[i]->thresholdCc) {
            sensors[i]->setThreshold(value);
        }

        if (control == sensors[i]->hysteresisCc) {
            sensors[i]->setHysteresis(value);
        }

        if (control == sensors[i]->retriggerCc) {
            sensors[i]->setRetriggerMs(value);
        }

        if (control == sensors[i]->readyStateCc) {
            sensors[i]->setReadyState(value > 0);
        }

        if (control == sensors[i]->chordStateCc) {
            sensors[i]->setChordState(value > 0);
        }
    }
}

void MidiInterface::sendControlChange(uint8_t control, uint8_t value) {
    usbMIDI.sendControlChange(control, value, MIDI_CHANNEL);
}
