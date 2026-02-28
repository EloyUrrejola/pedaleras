#include "MidiInterface.h"
#include "Sensor.h"

Adafruit_USBD_MIDI usbMIDI;
MidiInterface *midiInterfaceInstance = nullptr;

void midiControlChangeHandler(byte channel, byte control, byte value) {
    if (midiInterfaceInstance) {
        midiInterfaceInstance->handleControlChange(channel, control, value);
    }
}

MidiInterface::MidiInterface() {}

void MidiInterface::begin() {
    midiInterfaceInstance = this;
    usbMIDI.begin();
}

void MidiInterface::update() {
    uint8_t packet[4];

    while (usbMIDI.readPacket(packet)) {
        uint8_t status  = packet[1];
        uint8_t control = packet[2];
        uint8_t value   = packet[3];

        uint8_t type    = status & 0xF0;
        uint8_t channel = (status & 0x0F) + 1;

        if (type == 0xB0 && channel == MIDI_CHANNEL) {  
            handleControlChange(channel, control, value);
        }
    }
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
            sensors[i]->setChordState(value);
        }
    }
}

void MidiInterface::sendControlChange(uint8_t control, uint8_t value) {
    uint8_t packet[4];

    packet[0] = 0x0B;            // Cable number = 0, Code Index = 0xB (Control Change)
    packet[1] = 0xB0 | (MIDI_CHANNEL - 1); // Status byte: Control Change + canal
    packet[2] = control;         // Control number
    packet[3] = value;           // Valor del control

    usbMIDI.write(packet, 4);
}
