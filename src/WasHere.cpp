#include "WasHere.h"

MFRC522 identificator;

// Constructors

WasHere::WasHere () {
    WasHere::assignVariables("/application.json");
}

WasHere::WasHere (String directory) {
    WasHere::assignVariables(directory);
}

// Hidden functions

void WasHere::run () {

    // Init RFID

    identificator = MFRC522(WasHere::getSS(), WasHere::getRST());
    identificator.PCD_Init();

    // Fill wifi data

    WiFi.begin(WasHere::getSSID(), WasHere::getPassword());

    // Find method of the request

    std::function<int(HTTPClient&, String&)> method;

    if (WasHere::getMethod() == "GET") {
        method = [](HTTPClient& client, String& body) {
            return client.sendRequest("GET", body);
        };
    } 
            
    else if (WasHere::getMethod() == "POST") {
        method = [](HTTPClient& client, String& body) {
            return client.POST(body);
        };
    } 
            
    else if (WasHere::getMethod() == "PUT") {
        method = [](HTTPClient& client, String& body) {
            return client.PUT(body);
        };
    }

    else if (WasHere::getMethod() == "DELETE") {
        method = [](HTTPClient& client, String& body) {
            return client.sendRequest("DELETE", body);
        };
    }

    // Make a loop to change present of the people

    while (true) {
        WasHere::makePresent(method);
    }

}

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