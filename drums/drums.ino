#ifndef DRUMS_INO
#define DRUMS_INO

#define USB_MIDI_PRODUCT_NAME "MIDI Pads"

#include "src/Sensor.h"
#include "src/MidiInterface.h"
#include "src/Led.h"

const uint8_t NUMBER_OF_SENSORS = 1;

const uint8_t SENSOR_PINS[NUMBER_OF_SENSORS] = {14};
const uint8_t SENSOR_TRIGGER_CCS[NUMBER_OF_SENSORS] = {20};

const uint8_t SENSOR_READY_STATE_CCS[NUMBER_OF_SENSORS] = {21};
const uint8_t SENSOR_CHORD_STATE_CCS[NUMBER_OF_SENSORS] = {22};

const uint8_t SENSOR_THRESHOLD_CCS[NUMBER_OF_SENSORS] = {23};
const uint8_t SENSOR_HYSTERESIS_CCS[NUMBER_OF_SENSORS] = {24};
const uint8_t SENSOR_RETRIGGER_CCS[NUMBER_OF_SENSORS] = {25};

// Pines para LEDs
const uint8_t LED_READY_PINS[NUMBER_OF_SENSORS] = {24};   // Verde
const uint8_t LED_CHORD_PINS[NUMBER_OF_SENSORS] = {25};   // Azul
const uint8_t LED_BOUNCE_PINS[NUMBER_OF_SENSORS] = {28};  // Rojo
const uint8_t LED_CC_SEND_PINS[NUMBER_OF_SENSORS] = {29}; // Naranja

Sensor *sensors[NUMBER_OF_SENSORS];
MidiInterface *midiInterface;

// LEDs
Led *ledReady[NUMBER_OF_SENSORS];
Led *ledChord[NUMBER_OF_SENSORS];
Led *ledBounce[NUMBER_OF_SENSORS];
Led *ledCcSend[NUMBER_OF_SENSORS];

void setup()
{
    Serial.begin(9600);

    midiInterface = new MidiInterface();
    midiInterface->begin();

    // Primero registramos los sensores
    for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
        sensors[i] = new Sensor(
            SENSOR_PINS[i],
            SENSOR_TRIGGER_CCS[i],
            SENSOR_THRESHOLD_CCS[i],
            SENSOR_HYSTERESIS_CCS[i],
            SENSOR_RETRIGGER_CCS[i],
            SENSOR_READY_STATE_CCS[i],
            SENSOR_CHORD_STATE_CCS[i]
        );
    }

    midiInterface->registerSensors(sensors, NUMBER_OF_SENSORS);

    // Crear LEDs y asociarlos a cada sensor
    for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
        ledReady[i] = new Led(LED_READY_PINS[i]);
        ledChord[i] = new Led(LED_CHORD_PINS[i]);
        ledBounce[i] = new Led(LED_BOUNCE_PINS[i]);
        ledCcSend[i] = new Led(LED_CC_SEND_PINS[i]);

        sensors[i]->ledReady = ledReady[i];
        sensors[i]->ledChord = ledChord[i];
        sensors[i]->ledBounce = ledBounce[i];
        sensors[i]->ledCcSend = ledCcSend[i];
    }

    start();
}

void start()
{
    analogWrite(13, 255);
    delay(200);
    analogWrite(13, 0);
    delay(200);
    analogWrite(13, 255);
    delay(200);
    analogWrite(13, 0);
    delay(200);
    analogWrite(13, 255);
    delay(200);
    analogWrite(13, 0);
}

void loop()
{
    for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
        sensors[i]->update();
    }
    usbMIDI.read();
}

#endif
