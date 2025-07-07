#include "Device.h"
#include "LedActuator.h"
#include "ServoActuator.h"
#include "UltrasonicSensor.h"

class ParkingSpot : public Device
{
private:
  bool isOccupied = false;
  LedActuator ledRed;
  LedActuator ledGreen;
  LedActuator ledYellow;
  ServoActuator servo;
  UltrasonicSensor usSensor;
  bool isReserved = false;
  String spotId;
  String apiKey;
  String deviceLabel;

public:
  static const int LED_RED_PIN = 27;
  static const int LED_GREEN_PIN = 12;
  static const int LED_YELLOW_PIN = 25;
  static const int TRIG_SENSOR_PIN = 33;
  static const int ECHO_SENSOR_PIN = 32;
  static const int SERVO_PIN = 26;
  static const int RESERVED_SPOT_EVENT_ID = 2;
  static const int UNRESERVED_SPOT_EVENT_ID = 3;
  static const Event RESERVED_SPOT_EVENT;
  static const Event UNRESERVED_SPOT_EVENT;

  ParkingSpot(int ledRedPin = LED_RED_PIN, int ledGreenPin = LED_GREEN_PIN, int ledYellowPin = LED_YELLOW_PIN,
              int trigPin = TRIG_SENSOR_PIN, int echoPin = ECHO_SENSOR_PIN, int servoPin = SERVO_PIN);
  void on(Event event) override;
  void handle(Command command) override;
  void triggerSensorEvent(Event event);
  void measureDistance(float threshold = 20.0);
  bool getIsReserved();
  bool getIsOccupied();
  String getSpotId() const;
  String getApiKey() const;
  String getDeviceLabel() const;
  void setSpotId(const String &id);
  void setApiKey(const String &key);
  void setDeviceLabel(const String &label);
};