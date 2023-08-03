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
};

#endif