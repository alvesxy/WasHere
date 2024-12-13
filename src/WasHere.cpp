#include "WasHere.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include <FS.h>
#include <LittleFS.h>

#include <stdexcept>

// Constructors

WasHere::WasHere () {
    WasHere::assignVariables("/application.json");
}

WasHere::WasHere (String directory) {
    WasHere::assignVariables(directory);
}

// Hidden functions

void WasHere::assignVariables (String local) {

    // Open document
                
    if (!LittleFS.begin()) throw std::runtime_error("Cannot find file to apply configurations. Verify if the directory is in a folder **data** it should be in root of the project or you maked the Upload Filesystem Image correctly.");
                
    File file = LittleFS.open(local, "r");

    // Get content

    String content;

    while (file.available()) {
        content += (char)file.read();
    }
        
    file.close();

    // Keep in json object

    JsonDocument jsonDocument;

    DeserializationError error = deserializeJson(jsonDocument, content);

    // Assign to variables

    WasHere::setSS(jsonDocument["SS"]);
    WasHere::setRST(jsonDocument["RST"]);
    WasHere::setURL(jsonDocument["URL"]);
    WasHere::setMethod(jsonDocument["method"]);
    WasHere::setSSID(jsonDocument["SSID"]);
    WasHere::setPassword(jsonDocument["password"]);
    WasHere::setSecret(jsonDocument["secret"]);
    WasHere::setIssuer(jsonDocument["issuer"]);

    // Assign to states

    JsonArray states = jsonDocument["states"];

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