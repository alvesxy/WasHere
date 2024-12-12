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

#ifndef REQUEST_H
#define REQUEST_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <HTTPClient.h>

#include <functional>

#include <ArduinoJWT.h>

class Request {

    private:

        String url;
        std::function<int(HTTPClient&, String&)> type;
        String secret;

    public:

        Request () {}

        /**
         * Configure the future requests
         * @param url is the fullpath of the request (http://a.com/accept)
         * @param type of the request (GET, POST, PUT, DELETE)
         * @param ssid is the name when you search to wifis
         * @param password of to connect in wifi
         * @param secret is the key to backend side in json
        */
        Request (String url, String type, String ssid, String password, String secret) {

            this->url = url;
            this->secret = secret;
            
            if (type == "GET") {
                this->type = [](HTTPClient& client, String& body) {
                    return client.sendRequest("GET", body);
                };
            } 
            
            else if (type == "POST") {
                this->type = [](HTTPClient& client, String& body) {
                    return client.POST(body);
                };
            } 
            
            else if (type == "PUT") {
                this->type = [](HTTPClient& client, String& body) {
                    return client.PUT(body);
                };
            }

            else if (type == "DELETE") {
                this->type = [](HTTPClient& client, String& body) {
                    return client.sendRequest("DELETE", body);
                };
            }

            // Connect in wifi

            WiFi.begin(ssid, password);
            
        }

        /**
         * Make http requests
         * @param body is in format json to send in request
         * @return the status code of the response
        */
        int send (String body) {

            HTTPClient http;
            http.begin(url);

            http.addHeader("Content-Type", "application/json");

            const int status = this->type(http, body);

            http.end();

            return status;
        }

        /**
         * Make a String json with the identifier
         * @return For example: {"token": "fPfdlq352Jlfdac"}
        */
        String toJson (uint identifier) {

            ArduinoJWT jwt = ArduinoJWT(secret);

            JsonDocument json;

            json["identifier"] = identifier;
            json["iss"] = "com.github.alvesxy";

            String payload;
            serializeJson(json, payload);

            String token = jwt.encodeJWT(payload);

            json.clear();

            json["token"] = token;

            String encrypted;
            serializeJson(json, encrypted);

            return encrypted;
        }

};

#endif