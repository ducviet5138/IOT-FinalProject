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
        void SetUp();

        const char* GetChannel(String pram);
        void callback(char* topic, byte* message, unsigned int length);
        void reconnect();
        void ReconnectToServer();

        void Sync();
        void SyncTempAndHumid();

        void GetPersonStatus()
        
};

#endif