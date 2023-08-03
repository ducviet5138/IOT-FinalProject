#ifndef DEVICE_CONNECTION_H
#define DEVICE_CONNECTION_H

#include <WiFi.h>
#include <PubSubClient.h>

class DeviceConnection
{
    private:
        const char* server = "broker.hivemq.com";
        const String main_channel = "/GDrpD2J3jxvzQEy7vGOn/";
        WiFiClient espClient;
        PubSubClient client;
    
    public:
        void callback(char* topic, byte* message, unsigned int length);
{
    String buffer = "";
    for (int i = 0; i < length; i++) buffer += char(message[i]);

    Serial.println(buffer);
    working_mode = (buffer == "mode_on");
}
        DeviceConnection()
        {
            client = PubSubClient(espClient);
            client.setServer(server, 1883);
            client.setCallback(callback);
        }

        const char* GetChannel(String pram)
        {
            return (main_channel + pram).c_str();
        }
};


//
//;
#endif