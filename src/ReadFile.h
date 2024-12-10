#ifndef READFILE_H
#define READFILE_H

#include <Arduino.h>

#include <FS.h>
#include <LittleFS.h>

class ReadFile {

    private:

        String content;

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

            this->content = content;
        }

        // Getter

        String getContent () {
            return this->content;
        }

        // Setter

        void setContent (String content) {
            this->content = content;
        }
};

#endif