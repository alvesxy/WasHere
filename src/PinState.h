#ifndef PINSTATE_H
#define PINSTATE_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <vector>

class PinState {

    private:

        std::vector<std::function<void(int http)>> states = {};

    public:

        PinState () {}

        /**
         * Make a for each in the list states to handle correctly http status
         * @param json is the format base of the application.json
        */
        PinState (String json) {
              
            JsonDocument doc;

            DeserializationError error = deserializeJson(doc, json);

            JsonArray states = doc["states"];

            for (JsonObject state : states) {

                int pin = state["pin"];
                int status = state["status"];

                pinMode(pin, OUTPUT);

                this->states.push_back([pin, status](int http) {
                    
                    if (http == status) {
                        
                        digitalWrite(pin, HIGH);
                        delay(1000);
                        digitalWrite(pin, LOW);

                    }

                });

            }

        }

        /**
         * Turn on any led, it is corresponding in states
         * @param status is returned from the api
        */
        void explain (int status) {
            for (const auto& func : states) {
                func(status);
            }
        }
        
};

#endif