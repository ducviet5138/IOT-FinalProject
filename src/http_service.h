#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include <WiFi.h>

class HttpService
{
    private:
        const char *host = "api.thingspeak.com";
        const int port = 80;
        const char *request = "/update?api_key=FWA0NHRAFXF0M0FR";
    public:
        HttpService(){}
        
        String GetHTTPMessage(String param)
        {
            return  String("GET ") + request + param + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n\r\n";
        }

        void SendRequest(String param)
        {
            WiFiClient HttpClient;

            if (!HttpClient.connect(host, port))
            {
                Serial.println("Connecting to cloud...");
                delay(250);
            }

            HttpClient.print(GetHTTPMessage(param));
        }

        void GetRequest(String param)
        {
            
        }

        ~HttpService(){}
};

#endif