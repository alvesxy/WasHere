#ifndef WASHERE_H
#define WASHERE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoJWT.h>

#include <WiFi.h>
#include <HTTPClient.h>

#include <vector>

#include <functional>
#include <stdexcept>

#include <FS.h>
#include <LittleFS.h>

#include <MFRC522.h>

class WasHere {

    public:

        /**
         * Default constructor it only see the configurations in directory "/data/application.json"
        */
        WasHere ();
        
        /**
         * Constructor it accept any directory in the folder "/data/"
         * @param directory to find file in format json. For example, json/application.json
        */
        WasHere (String directory);

        /**
         * Run application to start loop
        */
        void run ();

    private:

        /**
         * Pin of the ESP32 it will connect with RFID's pin, probably called, **SDA**
        */
        uint8_t SS;

        /**
         * Pin to reset RFID
        */
        uint8_t RST;

        /**
         * The URL to send an request
        */
        String url;

        /**
         * The method to make the request
         * GET, POST, PUT or DELETE
        */
        String method;

        /**
         * SSID is the name of your wifi router thrown
         * When you search for wifis, you can choose one and write the password to connect
        */
        String ssid;

        /**
         * Password to connect in the web wifi
        */
        String password;

        /**
         * States of the pin will turn on a pin for few seconds, depending of the http status
         * For example, if the status be 404, the led RED will turn on to indicate it the people don't has list
        */
        std::vector<std::function<void(int http)>> states = {};

        /**
         * This is the password to decrypt the JWT in backend side
        */
        String secret;

        /** 
         * This is who sended the JWT to backend
        */
        String issuer;

    public:

        /**
         * Getter
        */

        uint8_t getSS () const {
            return SS;
        }

        uint8_t getRST () const {
            return RST;
        }

        String getURL () const {
            return url;
        }

        String getMethod () const {
            return method;
        }

        String getSSID () const {
            return ssid;
        }

        String getPassword () const {
            return password;
        }

        std::vector<std::function<void(int http)>> getStates() const {
            return states;
        }

        String getSecret() const {
            return secret;
        }

        String getIssuer() const {
            return issuer;
        }

        /**
         * Setter
        */

        void setSS (const uint8_t& SS) {
            this->SS = SS;
        }

        void setRST (const uint8_t& RST) {
            this->RST = RST;
        }

        void setURL (const String& url) {
            this->url = url;
        }

        void setMethod (const String& method) {
            this->method = method;
        }

        void setSSID (const String& ssid) {
            this->ssid = ssid;
        }

        void setPassword (const String& password) {
            this->password = password;
        }

        void setStates (const std::vector<std::function<void(int http)>>& states) {
            this->states = states;
        }

        void setSecret (const String& secret) {
            this->secret = secret;
        }

        void setIssuer (const String& issuer) {
            this->issuer = issuer;
        }

    private:

        /**
         * Hidden functions
        */

        /**
         * Function to read the file json and assign each value in a local variable
         * @param local is the directory to see the json with values
        */
        void assignVariables (String local);

        /**
         * Function to verify if the RFID readed a card and send a request
         * @param method of the request
         * GET, POST, PUT or DELETE
        */
        void makePresent (std::function<int(HTTPClient&, String&)> method);
};

#endif