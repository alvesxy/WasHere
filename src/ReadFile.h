/**
 * @link https://www.linkedin.com/in/c-alves-f?
 * 
 * Table to connect esp32 with rfid
 * 
 * ESP32 | RFID
 * 3V3 | 3.3v
 * GND | GND
 * defined in application.json as RST | RST
 * defined in application.json as SS | SDA
 * 19 | MISO
 * 23 | MOSI
 * 18 | SCK
*/

#ifndef READFILE_H
#define READFILE_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <FS.h>
#include <LittleFS.h>

class ReadFile {

    private:

        JsonDocument content;

    public:

        /**
         * Read a file with Littlefs
         * @param local is the directory until the file
        */
        ReadFile (String local) {
            
            if (!LittleFS.begin()) Serial.println("Failed to start the LittleFS in ReadJson.h");
                
            File file = LittleFS.open(local, "r");

            String content;

            while (file.available()) {
                content += (char)file.read();
            }
        
            file.close();

            DeserializationError error = deserializeJson(this->content, content);
        }

        // Getter

        JsonDocument getContent () {
            return this->content;
        }

        String getContentString () {
            
            String json;
            serializeJson(this->content, json);

            return json;
        }

        /**
         * @return the SDA pin of the RFID
        */
        int getSS () {
            return this->content["RFID"]["SS"];
        }

        /**
         * @return the RESET pin of the RFID
        */
        int getRST () {
            return this->content["RFID"]["RST"];
        }

        /**
         * @return the URL to make request
        */
        String getURL () {
            return this->content["route"]["URL"];
        }

        /**
         * @return the type of the request (GET, POST, PUT, DELETE, PATCH...)
        */
        String getType () {
            return this->content["route"]["type"];
        }

        /**
         * @return the ssid to connect in wifi, this is the name when you search
        */
        String getSSID () {
            return this->content["wifi"]["ssid"];
        }

        /**
         * @return the password to connect in wifi
        */
        String getPassword () {
            return this->content["wifi"]["password"];
        }

        /**
         * @return the secret key to the jwt
        */
        String getSecret () {
            return this->content["secret"];
        }

        // Setter

        void setContent (JsonDocument content) {
            this->content = content;
        }
};

#endif