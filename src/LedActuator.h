#include "Actuator.h"

class LedActuator: public Actuator {
  bool state;

  public: 
    static const int TOGGLE_LED_COMMAND_ID = 3; ///< Unique ID for toggle command.
    static const int TURN_ON_COMMAND_ID = 4; ///< Unique ID for turn-on command.
    static const int TURN_OFF_COMMAND_ID = 5; ///< Unique ID for turn-off command.
    static const Command TOGGLE_LED_COMMAND; ///< Predefined command to toggle the LED.
    static const Command TURN_ON_COMMAND; ///< Predefined command to turn the LED ON.
    static const Command TURN_OFF_COMMAND; ///< Predefined command to turn the LED OFF.

    LedActuator(int pin, bool initialState = false, CommandHandler* commandHandler = nullptr);
    
    void handle(Command command) override;

    bool getState() const;

    void setState(bool newState);
};