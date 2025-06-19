#include "ServoActuator.h"

const Command ServoActuator::TOGGLE_RESERVE_COMMAND = Command(TOGGLE_RESERVE_COMMAND_ID);
const Command ServoActuator::RESERVED_COMMAND = Command(RESERVED_COMMAND_ID);
const Command ServoActuator::UNRESERVED_COMMAND = Command(UNRESERVED_COMMAND_ID);

ServoActuator::ServoActuator(int pin, bool reserved, CommandHandler* commandHandler) 
  : Actuator(pin, commandHandler), reserved(reserved) {
  
  servo.attach(pin);
  servo.write(90);
}

void ServoActuator::handle(Command command) {
  if(command == TOGGLE_RESERVE_COMMAND) {
    reserved = !reserved;
    reserved ? servo.write(reservedPos) : servo.write(unreservedPos);
  } else if (command == RESERVED_COMMAND) {
    servo.write(reservedPos);
  } else if (command == UNRESERVED_COMMAND) {
    servo.write(unreservedPos);
  }

  Actuator::handle(command);
}

bool ServoActuator::getReserved() const {
  return reserved;
}

void ServoActuator::setReserved(bool newReserved) {
  reserved = newReserved;
  reserved ? servo.write(reservedPos) : servo.write(unreservedPos);
}