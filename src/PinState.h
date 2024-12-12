#ifndef PINSTATE_H
#define PINSTATE_H

#include <Arduino.h>
#include <ArduinoJson.h>

class PinState {

    public:

        /**
         * Make a for each in the list states to handle correctly http status
         * @param json is the format base of the application.json
        */
        PinState (String json) {
              
            JsonDocument doc;

            DeserializationError error = deserializeJson(doc, json);

            JsonArray states = doc["states"];

            for (JsonObject state : states) {
  
              int status = state["status"];
              int pin = state["pin"];

            }

        }
        
};

#endif