#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include <WiFi.h>

class HttpService
{
    private:
        const char * host = "www.example.com";
        const int port = 80;
        const char *request = "/";
    public:
        void SendRequest(String param)
        {
            WiFiClient HttpClient;

            while(!HttpClient.connect(host, port)) 
            {
                Serial.println("connection fail");
                delay(1000);
            }
            HttpClient.print(String("GET ") + request + " HTTP/1.1\r\n"
                        + "Host: " + host + "\r\n"
                        + "Connection: close\r\n\r\n");
            delay(500);

            while(HttpClient.available()) 
            {
                String line = HttpClient.readStringUntil('\r');
                Serial.print(line);
            }
        }

        ~HttpService()
        {}
};

#endif