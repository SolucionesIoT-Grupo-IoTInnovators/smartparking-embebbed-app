#include "Sensor.h"

Sensor::Sensor(EventHandler* eventHandler)
    : handler(eventHandler) {}

void Sensor::on(Event event) {
    if (handler != nullptr) {
        handler->on(event);
    }
}

void Sensor::setHandler(EventHandler* eventHandler) {
    handler = eventHandler;
}