#include <ESP32Servo.h>
#include "Actuator.h"

class ServoActuator : public Actuator {
private:
  Servo servo;
  const int reservedPos = 0;
  const int unreservedPos = 90;
  bool reserved;

public:
  static const int TOGGLE_RESERVE_COMMAND_ID = 0;
  static const int RESERVED_COMMAND_ID = 1;
  static const int UNRESERVED_COMMAND_ID = 2;
  static const Command TOGGLE_RESERVE_COMMAND;
  static const Command RESERVED_COMMAND;
  static const Command UNRESERVED_COMMAND;

  ServoActuator(int pin, bool reserved = false, CommandHandler* commandHandler = nullptr);
  void handle(Command command) override;
  bool getReserved() const;
  void setReserved(bool newReserved);
};