#include "UltrasonicSensor.h"

const Event UltrasonicSensor::THRESHOLD_LESS_SENSE_EVENT = Event(THRESHOLD_LESS_SENSE_EVENT_ID);
const Event UltrasonicSensor::THRESHOLD_MORE_SENSE_EVENT = Event(THRESHOLD_MORE_SENSE_EVENT_ID);

UltrasonicSensor::UltrasonicSensor(int trigPin, int echoPin, EventHandler* eventHandler)
  : Sensor(eventHandler), trigPin(trigPin), echoPin(echoPin) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

int UltrasonicSensor::getTrigPin() {
  return trigPin;
}

int UltrasonicSensor::getEchoPin() {
  return echoPin;
}