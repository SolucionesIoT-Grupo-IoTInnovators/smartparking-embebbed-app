#include "Sensor.h"
#include <Arduino.h>

class UltrasonicSensor : public Sensor {
private:
  int trigPin;
  int echoPin;
  
public:
  static const int THRESHOLD_LESS_SENSE_EVENT_ID = 0;
  static const Event THRESHOLD_LESS_SENSE_EVENT;
  static const int THRESHOLD_MORE_SENSE_EVENT_ID = 1;
  static const Event THRESHOLD_MORE_SENSE_EVENT;

  UltrasonicSensor(int trigPin, int echoPin, EventHandler* eventHandler = nullptr);
  int getTrigPin();
  int getEchoPin();
};