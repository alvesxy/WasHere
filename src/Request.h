#ifndef REQUEST_H
#define REQUEST_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <HTTPClient.h>

#include <functional>

class Request {

    private:

        String url;
        std::function<int(HTTPClient&, String&)> type;

    public:

        Request () {}

        /**
         * Configure the future requests
         * @param url is the fullpath of the request (http://a.com/accept)
         * @param type of the request (GET, POST, PUT, DELETE)
         * @param ssid is the name when you search to wifis
         * @param password of to connect in wifi
        */
        Request (String url, String type, String ssid, String password) {

            this->url = url;
            
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
         * @return {"id": identifier}
        */
        String toJson (uint identifier) {

            JsonDocument json;

            json["id"] = identifier;

            String output;
            serializeJson(json, output);

            return output;
        }

};

#endif