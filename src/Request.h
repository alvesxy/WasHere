#ifndef REQUEST_H
#define REQUEST_H

#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>

#include <functional>

class Request {

    private:

        String url;
        std::function<int(HTTPClient&, String&)> type;

    public:

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

};

#endif REQUEST_H