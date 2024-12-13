#ifndef WASHERE_H
#define WASHERE_H

#include <Arduino.h>
#include <vector>

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

};


#endif