#include "ParkingSpot.h"
#include <Arduino.h>

const Event ParkingSpot::RESERVED_SPOT_EVENT = Event(RESERVED_SPOT_EVENT_ID);
const Event ParkingSpot::UNRESERVED_SPOT_EVENT = Event(UNRESERVED_SPOT_EVENT_ID);

ParkingSpot::ParkingSpot(int ledRedPin, int ledGreenPin, int ledYellowPin, int trigPin, int echoPin, int servoPin)
    : ledRed(ledRedPin, false, this), ledGreen(ledGreenPin, true, this), ledYellow(ledYellowPin, false, this), usSensor(trigPin, echoPin, this), servo(servoPin, false, this)
{
}

void ParkingSpot::on(Event event)
{
  if (event == RESERVED_SPOT_EVENT)
  {
    Serial.println("RESERVED");
    isReserved = true;
    ledYellow.handle(LedActuator::TURN_ON_COMMAND);
    ledRed.handle(LedActuator::TURN_OFF_COMMAND);
    ledGreen.handle(LedActuator::TURN_OFF_COMMAND);
    servo.handle(ServoActuator::RESERVED_COMMAND);
  }
  else if (event == UNRESERVED_SPOT_EVENT)
  {
    isReserved = false;
    ledYellow.handle(LedActuator::TURN_OFF_COMMAND);
    servo.handle(ServoActuator::UNRESERVED_COMMAND);
    measureDistance();
  }
  else if (!isReserved)
  {
    if (event == UltrasonicSensor::THRESHOLD_LESS_SENSE_EVENT)
    {
      isOccupied = true;
      ledRed.handle(LedActuator::TURN_ON_COMMAND);
      ledGreen.handle(LedActuator::TURN_OFF_COMMAND);
    }
    else if (event == UltrasonicSensor::THRESHOLD_MORE_SENSE_EVENT)
    {
      isOccupied = false;
      ledRed.handle(LedActuator::TURN_OFF_COMMAND);
      ledGreen.handle(LedActuator::TURN_ON_COMMAND);
    }
  }
}

void ParkingSpot::handle(Command command)
{
}

void ParkingSpot::triggerSensorEvent(Event event)
{
  usSensor.on(event);
}

void ParkingSpot::measureDistance(float threshold)
{
  if (isReserved)
  {
    return;
  }

  digitalWrite(usSensor.getTrigPin(), LOW);
  delayMicroseconds(2);
  digitalWrite(usSensor.getTrigPin(), HIGH);
  delayMicroseconds(10);
  digitalWrite(usSensor.getTrigPin(), LOW);

  long duration = pulseIn(usSensor.getEchoPin(), HIGH);

  float distance = duration * 0.034 / 2;

  if (distance < threshold)
  {
    triggerSensorEvent(UltrasonicSensor::THRESHOLD_LESS_SENSE_EVENT);
  }
  else
  {
    triggerSensorEvent(UltrasonicSensor::THRESHOLD_MORE_SENSE_EVENT);
  }
}

bool ParkingSpot::getIsReserved()
{
  return isReserved;
}

bool ParkingSpot::getIsOccupied()
{
  return isOccupied;
}

String ParkingSpot::getSpotId() const
{
  return spotId;
}

String ParkingSpot::getApiKey() const
{
  return apiKey;
}

String ParkingSpot::getDeviceLabel() const
{
  return deviceLabel;
}

void ParkingSpot::setSpotId(const String &id)
{
  spotId = id;
}

void ParkingSpot::setApiKey(const String &key)
{
  apiKey = key;
}

void ParkingSpot::setDeviceLabel(const String &label)
{
  deviceLabel = label;
}
