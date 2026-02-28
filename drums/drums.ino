#ifndef DRUMS_INO
#define DRUMS_INO

#define USB_MIDI_PRODUCT_NAME "MIDI Pads"

#include "src/Sensor.h"
#include "src/MidiInterface.h"
#include "src/Led.h"

const uint8_t NUMBER_OF_SENSORS = 1;

const uint8_t SENSOR_TRIGGER_CCS[NUMBER_OF_SENSORS] = {20};

const uint8_t SENSOR_READY_STATE_CCS[NUMBER_OF_SENSORS] = {21};
const uint8_t SENSOR_CHORD_STATE_CCS[NUMBER_OF_SENSORS] = {22};

const uint8_t SENSOR_THRESHOLD_CCS[NUMBER_OF_SENSORS] = {23};
const uint8_t SENSOR_HYSTERESIS_CCS[NUMBER_OF_SENSORS] = {24};
const uint8_t SENSOR_RETRIGGER_CCS[NUMBER_OF_SENSORS] = {25};

// Pines sensores
const uint8_t SENSOR_PIN = 26;
const uint8_t SENSOR_PINS[NUMBER_OF_SENSORS] = {SENSOR_PIN};
// Pines para LEDs
const uint8_t GREEN_LED_PIN = 11;
const uint8_t BLUE_LED_PIN_1 = 12;
const uint8_t BLUE_LED_PIN_2 = 13;
const uint8_t YELLOW_LED_PIN = 14;

const uint8_t LED_READY_PINS[NUMBER_OF_SENSORS] = {GREEN_LED_PIN};
const uint8_t CHORD_LEDS_PER_SENSOR = 2;
const uint8_t LED_CHORD_PINS[NUMBER_OF_SENSORS][CHORD_LEDS_PER_SENSOR] = {{BLUE_LED_PIN_1, BLUE_LED_PIN_2}};
const uint8_t LED_CC_SEND_PINS[NUMBER_OF_SENSORS] = {YELLOW_LED_PIN};

Sensor *sensors[NUMBER_OF_SENSORS];
MidiInterface *midiInterface;

// LEDs
Led *ledReady[NUMBER_OF_SENSORS];
Led *ledChord[NUMBER_OF_SENSORS][CHORD_LEDS_PER_SENSOR];
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
        ledCcSend[i] = new Led(LED_CC_SEND_PINS[i]);
        for (uint8_t j = 0; j < CHORD_LEDS_PER_SENSOR; j++) {
            ledChord[i][j] = new Led(LED_CHORD_PINS[i][j]);
        }
        sensors[i]->setChordLeds(ledChord[i], CHORD_LEDS_PER_SENSOR);
        sensors[i]->ledReady = ledReady[i];
        sensors[i]->ledCcSend = ledCcSend[i];
    }

    start();
}

void start()
{
    for (uint8_t i = 0; i < 3; i++) {
        analogWrite(11, 255);
        delay(200);
        analogWrite(11, 0);
        analogWrite(12, 255);
        delay(200);
        analogWrite(12, 0);
        analogWrite(13, 255);
        delay(200);
        analogWrite(13, 0);
        analogWrite(14, 255);
        delay(200);
        analogWrite(14, 0);
    }
    for (uint8_t i = 0; i < 3; i++) {
        delay(200);
        analogWrite(11, 255);
        analogWrite(12, 255);
        analogWrite(13, 255);
        analogWrite(14, 255);
        delay(500);
        analogWrite(11, 0);
        analogWrite(12, 0);
        analogWrite(13, 0);
        analogWrite(14, 0);
    }
}

void loop()
{
    for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
        sensors[i]->update();
    }
    midiInterface->update();
}

#endif
