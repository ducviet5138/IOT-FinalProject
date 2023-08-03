#ifndef MY_DEVICE_H
#define MY_DEVICE_H

#include "device/device_dht.h"
#include "device/device_lcd.h"
#include "device/device_pir.h"
#include "device/device_relay.h"
#include "device/device_IR.h"
#include <PubSubClient.h>
#include <WiFi.h>

class MyDevice
{
    private:
        DeviceDHT dht;
        DeviceLCD lcd;
        DevicePIR pir;
        DeviceRelay relay;
        DeviceIR ir;

        bool working_mode;
        bool person_in_room;

        const char* server = "broker.hivemq.com";
        const String main_channel = "/GDrpD2J3jxvzQEy7vGOn/";
        WiFiClient espClient;
        PubSubClient client;
        
        const String ssid = "Wokwi-GUEST";
        const String pw = "";
    public:
        void SetUp()
        {
            dht.SetUp();
            lcd.SetUp();
            pir.SetUp();
            relay.SetUp();
            ir.SetUp();

            working_mode = 1;
            person_in_room = 1;

            client = PubSubClient(espClient);
            client.setServer(server, 1883);
            client.setCallback(callback);

            WiFi.begin(ssid, pw);
        }  

        void reconnect()
        {
            while (!client.connected())
            {
                String clientId = "ESP32Client-21127112-21127141-21127203";

                Serial.print("Connecting to server... ");
                if (client.connect(clientId.c_str()))
                {
                    Serial.println("Connected!");
                    client.subscribe(GetChannel("WorkingMode"));
                }
                else
                {
                    Serial.println("Failed!");
                    delay(5000);
                }
            }
        }


        void callback(char* topic, byte* message, unsigned int length);
        {
            String buffer = "";
            for (int i = 0; i < length; i++) buffer += char(message[i]);    
            Serial.println(buffer);
        }    

        const char* GetChannel(String pram)
        {
            return (main_channel + pram).c_str();
        } 

        void GetPersonStatus()
        {
            person_in_room = pir.IsMotion();
        }

        void Sync(String param, String value)
        {
            client.publish(GetChannel(pram), value.c_str());
        }

        void SyncTempAndHumid()
        {
            Sync("temperature", dht.GetTemperature());
            Sync("humid", dht.GetHumidity());
        }

        void ReconnectToServer()
        {
            if (!client.connected()) reconnect();
            client.loop();
        }
};

#endif