#include "LedActuator.h"
#include <Arduino.h>

const Command LedActuator::TOGGLE_LED_COMMAND = Command(TOGGLE_LED_COMMAND_ID);
const Command LedActuator::TURN_ON_COMMAND = Command(TURN_ON_COMMAND_ID);
const Command LedActuator::TURN_OFF_COMMAND = Command(TURN_OFF_COMMAND_ID);

LedActuator::LedActuator(int pin, bool initialState, CommandHandler* commandHandler)
    : Actuator(pin, commandHandler), state(initialState) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);
}

void LedActuator::handle(Command command) {
    if (command == TOGGLE_LED_COMMAND) {
        state = !state;
        digitalWrite(pin, state);
    } else if (command == TURN_ON_COMMAND) {
        state = true;
        digitalWrite(pin, state);
    } else if (command == TURN_OFF_COMMAND) {
        state = false;
        digitalWrite(pin, state);
    }
    Actuator::handle(command); // Propagate to handler if set
}

bool LedActuator::getState() const {
    return state;
}

void LedActuator::setState(bool newState) {
    state = newState;
    digitalWrite(pin, state);
}