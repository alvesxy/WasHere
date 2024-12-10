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

        // Setter

        void setContent (JsonDocument content) {
            this->content = content;
        }
};

#endif